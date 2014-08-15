// Copyright 2004-2014, North State Software, LLC.  All rights reserved.

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

#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <queue>
#include "NorthStateFramework.h"

namespace CommandProcessorWithRetryExample
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