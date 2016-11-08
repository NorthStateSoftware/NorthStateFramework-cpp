// Copyright 2004-2016, North State Software, LLC.  All rights reserved.

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "NSFTimerThread.h"

#include "NSFEvent.h"
#include "NSFExceptionHandler.h"
#include <limits.h>

namespace NorthStateFramework
{
    // Public

    NSFTimerThread::NSFTimerThread(const NSFString& name)
        : NSFThread(name, NSFOSThread::getHighestPriority()), maxAllowableTimeGap(5000), maxObservedTimeGap(0), timer(NSFOSTimer::create(name))
    {
        TimeGapActions.setExceptionAction(NSFAction(this, &NSFTimerThread::handleTimeGapActionException));

        startThread();
    }

    NSFTimerThread::~NSFTimerThread()
    {
        terminate(true);
        delete timer;
    }

    bool NSFTimerThread::isScheduled(NSFTimerAction* action)
    {
        LOCK(getThreadMutex())
        {
            return (std::find(actions.begin(), actions.end(), action) != actions.end());
        }
        ENDLOCK;
    }

    void NSFTimerThread::scheduleAction(NSFTimerAction* action)
    {
        LOCK(getThreadMutex())
        {
            // Do not schedule any actions if terminating or terminated (i.e. not ready)
            if (getTerminationStatus() != ThreadReady)
            {
                return;
            }

            insertAction(action);

            // Set next timeout if action was inserted in the front of the list
            if (action == actions.front())
            {
                timer->setNextTimeout(action->getExecutionTime());
            }
        }
        ENDLOCK;
    }

    void NSFTimerThread::scheduleAction(NSFTimerAction* action, NSFTime delayTime, NSFTime repeatTime)
    {
        action->setExecutionTime(getCurrentTime() + delayTime);
        action->setRepeatTime(repeatTime);
        action->setDelayTime(delayTime);

        scheduleAction(action);
    }

    void NSFTimerThread::terminate(bool waitForTerminated)
    {
        // Base class behavior, but return immediately so timer can be set to wake up timer thread
        NSFThread::terminate(false);

        LOCK(getThreadMutex())
        {
            timer->setNextTimeout(getCurrentTime());
        }
        ENDLOCK;

        // Wait as specified for thread to terminate after waking up
        NSFThread::terminate(waitForTerminated);
    }

    void NSFTimerThread::unscheduleAction(NSFTimerAction* action)
    {
        LOCK(getThreadMutex())
        {
            if (!actions.empty())
            {
                if (action == actions.front())
                {
                    actions.remove(action);

                    if (!actions.empty())
                    {
                        timer->setNextTimeout(actions.front()->getExecutionTime());
                    }
                }
                else
                {
                    actions.remove(action);
                }
            }
        }
        ENDLOCK;
    }

    // Private

    void NSFTimerThread::executeAction(NSFTimerAction* action)
    {
        // Guard a bad action from taking down timer thread
        try
        {
            action->execute();
        }
        catch(const std::exception& exception)
        {
            handleException(std::runtime_error(action->getName() + " action execution exception: " + exception.what()));
        }
        catch(...)
        {
            handleException(std::runtime_error(action->getName() + " action execution exception: unknown exception"));
        }
    }

    void NSFTimerThread::handleTimeGapActionException(const NSFExceptionContext& context)
    {
        handleException(std::runtime_error(getName() + " time gap action exception: " + context.getException().what()));
    }

    void NSFTimerThread::insertAction(NSFTimerAction* action)
    {
        // Make sure action is not already in list
        actions.remove(action);

        // Insert into list based on execution time order
        // Actions with equal execution times are executed in FIFO order
        std::list<NSFTimerAction*>::iterator actionIterator;
        for (actionIterator = actions.begin(); actionIterator != actions.end(); ++actionIterator)
        {
            if (action->getExecutionTime() < (*actionIterator)->getExecutionTime())
            {
                actions.insert(actionIterator, action);
                return;
            }
        }

        // Insert action at end of list if not alread inserted
        actions.push_back(action);
    }

    void NSFTimerThread::threadLoop()
    {
        NSFTime currentTime = getCurrentTime();
        std::list<NSFTimerAction*> readyActions;

        while (true)
        {
            // Set up next timeout
            LOCK(getThreadMutex())
            {
                if (!actions.empty())
                {
                    timer->setNextTimeout(actions.front()->getExecutionTime());
                }
                else
                {
                    timer->setNextTimeout(INT_MAX);
                }
            }
            ENDLOCK;

            timer->waitForNextTimeout();

            // Clean up and return if terminating
            if (getTerminationStatus() != ThreadReady)
            {
                actions.clear();
                return;
            }

            currentTime = getCurrentTime();

            LOCK(getThreadMutex())
            {
                // Create list of actions ready to execute
                std::list<NSFTimerAction*>::iterator actionIterator;
                for (actionIterator = actions.begin(); actionIterator != actions.end(); ++actionIterator)
                {
                    if ((*actionIterator)->getExecutionTime() <= currentTime)
                    {
                        readyActions.push_back((*actionIterator));
                    }
                    else
                    {
                        break;
                    }
                }

                // Reschedule repetitive actions
                for (actionIterator = readyActions.begin(); actionIterator != readyActions.end(); ++actionIterator)
                {
                    if ((*actionIterator)->getRepeatTime() != 0)
                    {
                        (*actionIterator)->setExecutionTime((*actionIterator)->getExecutionTime() + (*actionIterator)->getRepeatTime());
                        insertAction(*actionIterator);
                    }
                    else
                    {
                        actions.remove(*actionIterator);
                    }
                }
            }
            ENDLOCK;

            // Check for excessive gap between current time and execution time
            if (!readyActions.empty())
            {
                NSFTime timeGap = currentTime - readyActions.front()->getExecutionTime();

                if ((maxAllowableTimeGap > 0) && (timeGap > maxAllowableTimeGap))
                {
                    std::runtime_error newException(getName() + " time gap occurred: " + toString(timeGap));
                    handleException(newException);
                    TimeGapActions.execute(NSFExceptionContext(this, newException));
                }

                if (timeGap > maxObservedTimeGap)
                {
                    maxObservedTimeGap = timeGap;
                }
            }

            // Execute all ready actions
            while (!readyActions.empty())
            {
                executeAction(readyActions.front());
                readyActions.pop_front();
            }
        }
    }
}
