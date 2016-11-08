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

#include "CommandProcessorWithResetStateMachine.h"

namespace CommandProcessorWithResetStateMachineExample
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    CommandProcessorWithResetStateMachine::CommandProcessorWithResetStateMachine(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)), responseTimeout(1000), 

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        // Event constructors take the form (name, parent)
        // Data event constructors take the form (name, parent, data payload)
        newCommandEvent("NewCommand", this, "CommandPayload"),
        newResponseEvent("NewResponse", this, "ResponsePayload"),
        responseTimeoutEvent("ResponseTimeout", this),
        resetEvent("Reset", this),

        // Regions and states, from outer to inner 
        // Initial state construtors take the form (name, parent)
        initialCommandProcessorState("InitialCommandProcessor", this),
        // Composite state construtors take the form (name, parent, entry action, exit action)
        waitForCommandState("WaitForCommand", this, NULL, NULL),
        waitForResponseState("WaitForResponse", this, NSFAction(this, &CommandProcessorWithResetStateMachine::waitForResponseEntryActions), NSFAction(this, &CommandProcessorWithResetStateMachine::waitForResponseExitActions)),
        errorState("Error", this, NSFAction(this, &CommandProcessorWithResetStateMachine::errorEntryActions), NULL),
        resetState("Reset", this),

        // Transitions, ordered internal, local, external
        // Internal transition construtors take the form (name, state, trigger, guard, action)
        reactionToNewCommand("ReactionToNewCommand", this, &newCommandEvent, NULL, NSFAction(this, &CommandProcessorWithResetStateMachine::queueCommand)),
        // External transition construtors take the form (name, source, target, trigger, guard, action)
        initialCommandProcessorToWaitForCommandTransition("InitialToWaitForCommand", &initialCommandProcessorState, &waitForCommandState, NULL, NULL, NULL),
        waitForCommandToWaitForResponseTransition("WaitForCommandToWaitForResponse", &waitForCommandState, &waitForResponseState, NULL, NSFGuard(this, &CommandProcessorWithResetStateMachine::hasCommand), NSFAction(this, &CommandProcessorWithResetStateMachine::sendCommand)),
        waitForResponseToWaitForCommandTransition("WaitForResponseToWaitForCommand", &waitForResponseState, &waitForCommandState, &newResponseEvent, NSFGuard(this, &CommandProcessorWithResetStateMachine::isResponse), NSFAction(this, &CommandProcessorWithResetStateMachine::handleResponse)),
        waitForResponseToErrorTransition("WaitForResponseToError", &waitForResponseState, &errorState, &responseTimeoutEvent, NULL, NULL),
        errorToResetTransition("ErrorToReset", &errorState, &resetState, &resetEvent, NULL, NULL),
        resetToWaitForCommandTransition("ResetToWaitForCommand", &resetState, &waitForCommandState, NULL, NSFGuard(this, &CommandProcessorWithResetStateMachine::isReady), NULL)
    {
    }

    CommandProcessorWithResetStateMachine::~CommandProcessorWithResetStateMachine()
    {
        // It is good practice to stop event processing (terminate) in the 
        // destructor of a state machine or event handler to prevent 
        // entry/exit actions from being performed on a destructing object.
        terminate(true);
        delete getEventThread();
    }

    void CommandProcessorWithResetStateMachine::addCommand(const string& newCommand)
    {
        // Queue the event and return so as to not block the calling thread.
        // The event will be picked up by handleNewCommand() and added to a queue.
        // This approach eliminates the need to mutex the command queue,
        // because all queue manipulation occurs on the state machine thread.

        // It is not always necesary to copy an event before it is queued. 
        // However, in this case, multiple copies of the event can be queued,
        // with each event carrying a unique data payload.

        // Also note that event is being tagged as "deleteAfterHandling,"  that 
        // lets the framework know to delete this copy once processing is complete.
        queueEvent(newCommandEvent.copy(true, newCommand));
    }

    void CommandProcessorWithResetStateMachine::addResponse(const string& newResponse)
    {
        // Queue the event and return so as to not block the calling thread.
        queueEvent(newResponseEvent.copy(true, newResponse));
    }

    void CommandProcessorWithResetStateMachine::resetError()
    {
        queueEvent(&resetEvent);
    }

    void CommandProcessorWithResetStateMachine::queueCommand(const NSFStateMachineContext& context)
    {
        // Add data to command queue.
        // If the state machine is in the waitForCommandState, then the
        // run to completion step will result in transitioning to the
        // waitForResponse state, sending the command during the transtion.
        commandQueue.push(((NSFDataEvent<string>*)(context.getTrigger()))->getData());
    }

    bool CommandProcessorWithResetStateMachine::hasCommand(const NSFStateMachineContext& context)
    {
        return (!commandQueue.empty());
    }

    void CommandProcessorWithResetStateMachine::sendCommand(const NSFStateMachineContext& context)
    {
        // Code to send the command goes here
        // ...

        // Log trace of command sent
        // Trace Format:
        // <MessageSent>
        //   <Source>name</Source>
        //   <Message>message</Message>
        // </MessageSent>
        NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::MessageSentTag(), NSFTraceTags::SourceTag(), this->getName(), NSFTraceTags::MessageTag(), commandQueue.front());
        commandQueue.pop();
    }

    void CommandProcessorWithResetStateMachine::waitForResponseEntryActions(const NSFStateMachineContext& context)
    {
        // Schedule timeout event, in case no response received           
        responseTimeoutEvent.schedule(responseTimeout, 0);
    }

    void CommandProcessorWithResetStateMachine::waitForResponseExitActions(const NSFStateMachineContext& context)
    {
        // Unschedule the timeout event
        responseTimeoutEvent.unschedule();
    }

    bool CommandProcessorWithResetStateMachine::isResponse(const NSFStateMachineContext& context)
    {
        // Code to verify that the resposne is correct for the command goes here.
        // ...

        return true;
    }

    void CommandProcessorWithResetStateMachine::handleResponse(const NSFStateMachineContext& context)
    {
        // Code to handle the response goes here
        // ...

        // Log trace of response received
        // Trace Format:
        // <MessageReceived>
        //   <Source>name</Source>
        //   <Message>message</Message>
        // </MessageReceived>
        NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::MessageReceivedTag(), NSFTraceTags::SourceTag(), this->getName(), NSFTraceTags::MessageTag(), ((NSFDataEvent<string>*)(context.getTrigger()))->getData());
    }

    void CommandProcessorWithResetStateMachine::errorEntryActions(const NSFStateMachineContext& context)
    {
        // Code to handle the error goes here
        // ...
    }

    bool CommandProcessorWithResetStateMachine::isReady(const NSFStateMachineContext& context)
    {
        return resetState.getReadyState().isActive();
    }
}
