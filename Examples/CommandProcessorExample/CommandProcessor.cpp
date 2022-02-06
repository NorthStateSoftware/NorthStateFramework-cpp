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

#include "CommandProcessor.h"

namespace CommandProcessorExample
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    CommandProcessor::CommandProcessor(const NSFString& name)
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
        waitForResponseState("WaitForResponse", this, NSFAction(this, &CommandProcessor::waitForResponseEntryActions), NSFAction(this, &CommandProcessor::waitForResponseExitActions)),
        errorState("Error", this, NSFAction(this, &CommandProcessor::errorEntryActions), NULL),
        resetState("Reset", this, NSFAction(this, &CommandProcessor::resetEntryActions), NULL),

        // Transitions, ordered internal, local, external
        // Internal transition construtors take the form (name, state, trigger, guard, action)
        reactionToNewCommand("ReactionToNewCommand", this, &newCommandEvent, NULL, NSFAction(this, &CommandProcessor::queueCommand)),
        // External transition construtors take the form (name, source, target, trigger, guard, action)
        initialCommandProcessorToWaitForCommandTransition("InitialToWaitForCommand", &initialCommandProcessorState, &waitForCommandState, NULL, NULL, NULL),
        waitForCommandToWaitForResponseTransition("WaitForCommandToWaitForResponse", &waitForCommandState, &waitForResponseState, NULL, NSFGuard(this, &CommandProcessor::hasCommand), NSFAction(this, &CommandProcessor::sendCommand)),
        waitForResponseToWaitForCommandTransition("WaitForResponseToWaitForCommand", &waitForResponseState, &waitForCommandState, &newResponseEvent, NSFGuard(this, &CommandProcessor::isResponse), NSFAction(this, &CommandProcessor::handleResponse)),
        waitForResponseToErrorTransition("WaitForResponseToError", &waitForResponseState, &errorState, &responseTimeoutEvent, NULL, NULL),
        errorToResetTransition("ErrorToReset", &errorState, &resetState, &resetEvent, NULL, NULL),
        resetToWaitForCommandTransition("ResetToWaitForCommand", &resetState, &waitForCommandState, NULL, NSFGuard(this, &CommandProcessor::isReady), NULL)
    {
    }

    CommandProcessor::~CommandProcessor()
    {
        // It is good practice to stop event processing (terminate) in the 
        // destructor of a state machine or event handler to prevent 
        // entry/exit actions from being performed on a destructing object.
        terminate(true);
        delete getEventThread();
    }

    void CommandProcessor::addCommand(const string& newCommand)
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

    void CommandProcessor::addResponse(const string& newResponse)
    {
        // Queue the event and return so as to not block the calling thread.
        queueEvent(newResponseEvent.copy(true, newResponse));
    }

    void CommandProcessor::resetError()
    {
        queueEvent(&resetEvent);
    }

    void CommandProcessor::queueCommand(const NSFStateMachineContext& context)
    {
        // Add data to command queue.
        // If the state machine is in the waitForCommandState, then the
        // run to completion step will result in transitioning to the
        // waitForResponse state, sending the command during the transtion.
        commandQueue.push(((NSFDataEvent<string>*)(context.getTrigger()))->getData());
    }

    bool CommandProcessor::hasCommand(const NSFStateMachineContext&)
    {
        return (!commandQueue.empty());
    }

    void CommandProcessor::sendCommand(const NSFStateMachineContext&)
    {
        string commandString = commandQueue.front();

        // Code to send the command goes here
        // ...

        // Log trace of command sent
        // Trace Format:
        // <MessageSent>
        //   <Source>name</Source>
        //   <Message>message</Message>
        // </MessageSent>
        NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::MessageSentTag(), NSFTraceTags::SourceTag(), this->getName(), NSFTraceTags::MessageTag(), commandString);
    }

    void CommandProcessor::waitForResponseEntryActions(const NSFStateMachineContext&)
    {
        // Schedule timeout event, in case no response received           
        responseTimeoutEvent.schedule(responseTimeout, 0);
    }

    void CommandProcessor::waitForResponseExitActions(const NSFStateMachineContext&)
    {
        // Unschedule the timeout event
        responseTimeoutEvent.unschedule();

        commandQueue.pop();
    }

    bool CommandProcessor::isResponse(const NSFStateMachineContext&)
    {
        // Code to verify that the resposne is correct for the command goes here.
        // ...

        return true;
    }

    void CommandProcessor::handleResponse(const NSFStateMachineContext& context)
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

    void CommandProcessor::errorEntryActions(const NSFStateMachineContext&)
    {
        // Code to handle the error goes here
        // ...
    }

    void CommandProcessor::resetEntryActions(const NSFStateMachineContext&)
    {
        // Code to reset hardware goes here
        // ...
    }

    bool CommandProcessor::isReady(const NSFStateMachineContext&)
    {
        // Code to check if hardware is reset properly goes here
        // ...

        return true;
    }
}
