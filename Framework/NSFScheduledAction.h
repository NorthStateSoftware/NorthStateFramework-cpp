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

#ifndef NSF_SCHEDULED_ACTION_H
#define NSF_SCHEDULED_ACTION_H

#include "NSFDelegates.h"
#include "NSFEventHandler.h"
#include "NSFTimerAction.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an action or actions that can be scheduled for execution on a specified thread at a specified time.
    /// </summary>
    /// <remarks>
    /// Use this class to encapsulate a delegate or delegates so that they can be
    /// scheduled with the NSFOSTimer.  Actions execute on the specified event thread.
    /// </remarks>
    class NSFScheduledAction : public NSFTimerAction
    {
    public:

        /// <summary>
        /// Creates a scheduled action.
        /// </summary>
        /// <param name="name">The name of the scheduled action.</param>
        /// <param name="action">The action to execute.</param>
        /// <param name="eventThread">The thread on which the action will execute.</param>
        /// <remarks>
        /// Use null or String.Empty for the name if no name is desired.
        /// The action will be logged in the trace if the name is anything other than null or String.Empty.
        /// </remarks>
        NSFScheduledAction(const NSFString& name, const NSFVoidAction<NSFContext>& action, NSFEventThread* eventThread);

        /// <summary>
        /// Creates a scheduled action.
        /// </summary>
        /// <param name="name">The name of the scheduled action.</param>
        /// <param name="action">The action to execute.</param>
        /// <param name="eventThread">The thread on which the action will execute.</param>
        /// <remarks>
        /// Use null or String.Empty for the name if no name is desired.
        /// The action will be logged in the trace if the name is anything other than null or String.Empty.
        /// </remarks>
        NSFScheduledAction(const NSFString& name, const NSFVoidAction<NSFContext>* action, NSFEventThread* eventThread);

        /// <summary>
        /// Actions to be executed at the scheduled time.
        /// </summary>
        NSFVoidActions<NSFContext> Actions;

        /// <summary>
        /// Gets the event thread that the actions will execute on.
        /// </summary>
        NSFEventThread* getEventThread() const { return eventHandler.getEventThread(); }

    private:

        NSFEventHandler eventHandler;
        NSFEvent executeActionsEvent;

        /// <summary>
        /// Performs common construction behaviors.
        /// </summary>
        void construct();

        /// <summary>
        /// Callback method supporting NSFTimerAction interface.
        /// </summary>
        /// <remarks>
        /// This method is called by the NSFTimerThread to execute the actions at the scheduled time.
        /// </remarks>
        virtual void execute();

        /// <summary>
        /// Event handler action that executes the scheduled action.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void executeActions(const NSFEventContext& context);

        /// <summary>
        /// Handles exceptions caught during action execution.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void handleActionException(const NSFExceptionContext& context);
    };
}

#endif // NSF_SCHEDULED_ACTION_H
