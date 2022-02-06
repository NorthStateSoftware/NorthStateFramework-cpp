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

#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <queue>
#include "NorthStateFramework.h"

namespace CommandProcessorWithResetStrategyExample
{
    class CommandProcessor : public NSFStateMachine
    {
    public:

        CommandProcessor(const NSFString& name);

        ~CommandProcessor();

        // Public interface
        void addCommand(const string& newCommand);
        void addResponse(const string& newResponse);
        void resetError();

        // States and transitions may be exposed as part of the public interface, 
        // so other objects can attach entry or exit actions.
        NSFState& getWaitForCommandState() { return waitForCommandState; };
        NSFState& getWaitForResponseState() { return waitForResponseState; };
        NSFState& getErrorState() { return errorState; };
        NSFState& getResetState() { return resetState; };
        NSFExternalTransition& getWaitForCommandToWaitForResponseTransition() { return waitForCommandToWaitForResponseTransition; };
        NSFExternalTransition& getWaitForResponseToWaitForCommandTransition() { return waitForResponseToWaitForCommandTransition; };
        NSFExternalTransition& getWaitForResponseToErrorTransition() { return waitForResponseToErrorTransition; };
        NSFExternalTransition& getErrorToResetTransition() { return errorToResetTransition; };
        NSFExternalTransition& getResetToWaitForCommandTransition() { return resetToWaitForCommandTransition; };

    protected:

        queue<string> commandQueue;
        long responseTimeout;

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        NSFDataEvent<string> newCommandEvent;
        NSFDataEvent<string> newResponseEvent;
        NSFEvent responseTimeoutEvent;
        NSFEvent resetEvent;

        // Regions and states, from outer to inner
        NSFInitialState initialCommandProcessorState;
        NSFCompositeState waitForCommandState;
        NSFCompositeState waitForResponseState;
        NSFCompositeState errorState;
        NSFCompositeState resetState;

        // Transitions, ordered internal, local, external
        NSFInternalTransition reactionToNewCommand;
        NSFExternalTransition initialCommandProcessorToWaitForCommandTransition;
        NSFExternalTransition waitForCommandToWaitForResponseTransition;
        NSFExternalTransition waitForResponseToWaitForCommandTransition;
        NSFExternalTransition waitForResponseToErrorTransition;
        NSFExternalTransition errorToResetTransition;
        NSFExternalTransition resetToWaitForCommandTransition;

        // State machine guards and actions
        void queueCommand(const NSFStateMachineContext& context);

        bool hasCommand(const NSFStateMachineContext& context);
        void sendCommand(const NSFStateMachineContext& context);

        void waitForResponseEntryActions(const NSFStateMachineContext& context);
        void waitForResponseExitActions(const NSFStateMachineContext& context);
        bool isResponse(const NSFStateMachineContext& context);
        void handleResponse(const NSFStateMachineContext& context);

        void errorEntryActions(const NSFStateMachineContext& context);
        void resetEntryActions(const NSFStateMachineContext& context);
        bool isReady(const NSFStateMachineContext& context);
    };
}

#endif // COMMAND_PROCESSOR_H