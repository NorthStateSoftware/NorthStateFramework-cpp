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

#include "NSFScheduledAction.h"

#include "NSFExceptionHandler.h"
#include "NSFTraceLog.h"

namespace NorthStateFramework
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    // Public 

    NSFScheduledAction::NSFScheduledAction(const NSFString& name, const NSFVoidAction<NSFContext>& action, NSFEventThread* eventThread)
        : NSFTimerAction(name), Actions(&action), eventHandler(name, eventThread), executeActionsEvent(name, this, &eventHandler)
    {
        construct();
    }

    NSFScheduledAction::NSFScheduledAction(const NSFString& name, const NSFVoidAction<NSFContext>* action, NSFEventThread* eventThread)
        : NSFTimerAction(name), Actions(action), eventHandler(name, eventThread), executeActionsEvent(name, this, &eventHandler)
    {
        construct();
    }

    // Private

    void NSFScheduledAction::construct()
    {
        Actions.setExceptionAction(NSFAction(this, &NSFScheduledAction::handleActionException));

        eventHandler.setLoggingEnabled(false);
        eventHandler.addEventReaction(&executeActionsEvent, NSFAction(this, &NSFScheduledAction::executeActions));
    }

    void NSFScheduledAction::execute()
    {
        // Queue up event to be handled by the event handler on its thread.
        eventHandler.queueEvent(&executeActionsEvent);
    }

    void NSFScheduledAction::executeActions(const NSFEventContext& context)
    {
        if (!getName().empty())
        {
            NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::ActionExecutedTag(), NSFTraceTags::ActionTag(), getName());
        }

        Actions.execute(context);
    }

    void NSFScheduledAction::handleActionException(const NSFExceptionContext& context)
    {
        std::runtime_error newException(getName() + " action exception: " + context.getException().what());
        NSFExceptionHandler::getExceptionHandler().handleException(NSFExceptionContext(this, newException));
    }
}
