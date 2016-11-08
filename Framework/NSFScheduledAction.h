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
