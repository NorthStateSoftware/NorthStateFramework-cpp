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

#ifndef NSF_TIMER_ACTION_H
#define NSF_TIMER_ACTION_H

#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents the base class functionality for implementing a timer action.
    /// </summary>
    /// <remarks>
    /// Timer actions must be short in duration and must not block, as they are called directly from the timer thread.
    /// Concrete timer actions in the framework are NSFEvent, NSFScheduledAction, and NSFOSSignal.
    /// </remarks>
    class NSFTimerAction : public INSFNamedObject
    {
    public:

        friend class NSFTimerThread;

        /// <summary>
        /// Destroys a timer action.
        /// </summary>
        virtual ~NSFTimerAction();

        /// <summary>
        /// Gets the delay time of the action.
        /// </summary>
        NSFTime getDelayTime() const { return delayTime; }

        /// <summary>
        /// Sets the delay time of the action.
        /// </summary>
        void setDelayTime(NSFTime value) { delayTime = value; }

        /// <summary>
        /// Gets the execution time of the action.
        /// </summary>
        NSFTime getExecutionTime() const { return executionTime; }

        /// <summary>
        /// Sets the execution time of the action.
        /// </summary>
        void setExecutionTime(NSFTime value) { executionTime = value; }

        /// <summary>
        /// Gets the name of the action.
        /// </summary>
        virtual const NSFString& getName() const { return name; }

        /// <summary>
        /// Sets the name of the action.
        /// </summary>
        virtual void setName(const NSFString& value) { name = value.c_str(); }

        /// <summary>
        /// Gets the repeat time for periodic actions, 0 if non-periodic.
        /// </summary>
        NSFTime getRepeatTime() const { return repeatTime; }

        /// <summary>
        /// Sets the repeat time for periodic actions, 0 if non-periodic.
        /// </summary>
        void setRepeatTime(NSFTime value) { repeatTime = value; }

        /// <summary>
        /// Checks if the action is already scheduled.
        /// </summary>
        /// <returns>True if scheduled, otherwise false</returns>
        bool isScheduled();

        /// <summary>
        /// Schedules the action to execute at the previously designated delay and repeat times.
        /// </summary>
        void schedule();

        /// <summary>
        /// Schedules the action to execute after the specified delay time with zero repeat time.
        /// </summary>
        /// <param name="delayTime">The delay time until the action executes.</param>
        void schedule(NSFTime delayTime);

        /// <summary>
        /// Schedules the action to execute at the specified times.
        /// </summary>
        /// <param name="delayTime">The delay time until the action executes.</param>
        /// <param name="repeatTime">The repeat time for periodic actions, 0 if non-periodic.</param>
        void schedule(NSFTime delayTime, NSFTime repeatTime);

        /// <summary>
        /// Schedules the action to execute at its designated execution time.
        /// </summary>
        void scheduleAbsoluteExecution();

        /// <summary>
        /// Schedules the action to execute at the specified execution time.
        /// </summary>
        /// <param name="executionTime">The time the action executes.</param>
        void scheduleAbsoluteExecution(NSFTime executionTime);

        /// <summary>
        /// Schedules the action to execute at the specified execution time.
        /// </summary>
        /// <param name="executionTime">The time the action executes.</param>
        /// <param name="repeatTime">The repeat time for periodic actions, 0 if non-periodic.</param>
        void scheduleAbsoluteExecution(NSFTime executionTime, NSFTime repeatTime);

        /// <summary>
        /// Unschedules the action.
        /// </summary>
        void unschedule();

    protected:

        /// <summary>
        /// Creates a timer action.
        /// </summary>
        NSFTimerAction(const NSFString& name);

    private:

        NSFString name;
        NSFTime delayTime;
        NSFTime executionTime;
        NSFTime repeatTime;

        /// <summary>
        /// Callback method called by timer at expiration time.
        /// </summary>
        virtual void execute() = 0;
    };
}

#endif // NSF_TIMER_ACTION_H
