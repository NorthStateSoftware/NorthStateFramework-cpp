// MIT License

// North State Framework
// Copyright (c) 2004-2022 North State Software, LLC

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "NSFTimerThread.h"

#include "NSFEvent.h"
#include "NSFExceptionHandler.h"
#include "NSFTraceLog.h"
#include <limits.h>

namespace NorthStateFramework
{
    // Public

    NSFTimerThread::NSFTimerThread(const NSFString& name)
        : NSFThread(name, NSFOSThread::getHighestPriority()), maxAllowableTimeGap(5000), maxObservedTimeGap(0), nextTimeGapInterval(0), timer(NSFOSTimer::create(name))
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

                if ((maxAllowableTimeGap > 0) && (timeGap > maxAllowableTimeGap) && (currentTime > nextTimeGapInterval))
                {
                    NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::ErrorTag(), NSFTraceTags::SourceTag(), getName(), NSFTraceTags::MessageTag(), "TimeGap", NSFTraceTags::ValueTag(), toString<NSFTime>(timeGap));
                    TimeGapActions.execute(NSFContext(this));

                    // Set time when next time gap can be recorded
                    // This prevents all gapped timer events from recording time gap trace
                    nextTimeGapInterval = currentTime + maxAllowableTimeGap;
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
