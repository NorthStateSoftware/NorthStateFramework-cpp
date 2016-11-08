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

#ifndef NSF_TIMER_THREAD_H
#define NSF_TIMER_THREAD_H

#include "NSFTaggedTypes.h"
#include "NSFThread.h"
#include "NSFOSTimer.h"
#include "NSFTimerAction.h"
#include "NSFOSThread.h"


namespace NorthStateFramework
{
    /// <summary>
    /// Represents a timer thread.
    /// </summary>
    /// <summary>
    /// A timer thread contains a list of timer actions to execute at specified times.
    /// </summary>
    class NSFTimerThread : public NSFThread
    {
    public:

        /// <summary>
        /// Creates a timer thread.
        /// </summary>
        /// <param name="name">User specified name for timer.</param>
        NSFTimerThread(const NSFString& name);

        /// <summary>
        /// Destroys a timer thread.
        /// </summary>
        /// <remarks>
        /// This destructor will block for a short period until the thread is terminated.
        /// </remarks>
        virtual ~NSFTimerThread();

        /// <summary>
        /// Actions to be executed whenever the timer encounters delay greater than MaxAllowableTimeGap in processing actions.
        /// </summary>
        /// <remarks>
        /// In a well behaved system this event should not fire.  It is provided for diagnostic purposes.
        /// </remarks>
        NSFVoidActions<NSFContext> TimeGapActions;

        /// <summary>
        /// Gets the current time.
        /// </summary>
        NSFTime getCurrentTime() const { return getTimer()->getCurrentTime(); }

        /// <summary>
        /// Gets the maximum allowable delay the timer can see before firing the TimeGapActions.
        /// </summary>
        int getMaxAllowableTimeGap() const { return maxAllowableTimeGap; }

        /// <summary>
        /// Sets the maximum allowable delay the timer can see before firing the TimeGapActions.
        /// </summary>
        /// <param name="value">The time in milli-seconds.  Set to zero to disable.</param>
        void setMaxAllowableTimeGap(int value) { maxAllowableTimeGap = value; }

        /// <summary>
        /// Gets the maximum observed delay in executing timer actions.
        /// </summary>
        NSFTime getMaxObservedTimeGap() const { return maxObservedTimeGap; }

        /// <summary>
        /// Gets the underlying timer.
        /// </summary>
        NSFOSTimer* getTimer() const { return timer; }

        /// <summary>
        /// Gets the primary timer of the North State Framework.
        /// </summary>
        /// <remarks>
        /// Although it is possible to create additional timers, this is very rare and should be carefully considered if it is necessary.
        /// </remarks>
        static NSFTimerThread& getPrimaryTimerThread() { static NSFTimerThread nsfTimer("PrimaryTimerThread"); return nsfTimer; }

        /// <summary>
        /// Checks if an action is scheduled.
        /// </summary>
        /// <param name="action">The action in question.</param>
        /// <returns>
        /// True if the action is scheduled, otherwise false.
        /// </returns>
        bool isScheduled(NSFTimerAction* action);

        /// <summary>
        /// Schedules an action to execute at its previously designated execution time.
        /// </summary>
        /// <param name="action">The action to schedule.</param>
        void scheduleAction(NSFTimerAction* action);

        /// <summary>
        /// Schedules an action with the timer.
        /// </summary>
        /// <param name="action">The action to schedule.</param>
        /// <param name="delayTime">The delay time before the action should execute.</param>
        /// <param name="repeatTime">The repeat time if the action is periodic, or 0 if the action is non-periodic.</param>
        void scheduleAction(NSFTimerAction* action, NSFTime delayTime, NSFTime repeatTime);

        virtual void terminate(bool waitForTerminated);

        /// <summary>
        /// Unschedules a previously scheduled action.
        /// </summary>
        /// <param name="action">The action to unschedule.</param>
        /// <remarks>
        /// Unscheduling an action that is not currently scheduled has no effect.
        /// </remarks>
        void unscheduleAction(NSFTimerAction* action);

    private:

        std::list<NSFTimerAction*> actions;
        UInt32 maxAllowableTimeGap;
        NSFTime maxObservedTimeGap;
        NSFOSTimer* timer;

        /// <summary>
        /// Executes the specified action, and reschedules it if a repeat time is specified.
        /// </summary>
        /// <param name="action">The action to execute.</param>
        void executeAction(NSFTimerAction* action);

        /// <summary>
        /// Handles an exception caught while executing time gap actions.
        /// </summary>
        /// <param name="context">The exception context.</param>
        void handleTimeGapActionException(const NSFExceptionContext& context);

        /// <summary>
        /// Inserts an action into the action list based on its execution time.
        /// </summary>
        /// <param name="action">The action to insert.</param>
        /// <remarks>
        /// Inserts the action without mutex protection.
        /// </remarks>
        void insertAction(NSFTimerAction* action);

        virtual void threadLoop();
    };
}

#endif // NSF_TIMER_THREAD_H
