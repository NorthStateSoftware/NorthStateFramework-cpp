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

#include "NSFScheduledAction.h"

#include "NSFExceptionHandler.h"
#include "NSFTraceLog.h"

namespace NorthStateFramework
{
    // Public 

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif

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

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( pop )
#endif

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
