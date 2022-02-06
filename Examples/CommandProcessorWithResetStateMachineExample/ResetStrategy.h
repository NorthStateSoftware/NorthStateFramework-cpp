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

#ifndef RESET_STRATEGY_H
#define RESET_STRATEGY_H

#include "NorthStateFramework.h"

using namespace std;

namespace CommandProcessorWithResetStateMachineExample
{
    class ResetStrategy : public NSFStateMachine
    {
    public:

        ResetStrategy(const NSFString& name, NSFCompositeState* parentState);
        ~ResetStrategy();

        // States and transitions may be exposed as part of the public interface, 
        // so other objects can attach entry or exit actions.
        NSFState& getResetHardwareState() { return resetHardwareState; };
        NSFState& getReestablishCommunicationsState() { return reestablishCommunicationsState; };
        NSFState& getReadyState() { return readyState; };

    private:

        // Data members
        bool hardwareReset;
        bool commReady;
        int hardwareResetDelayTime;
        int commReadyDelayTime;

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        NSFEvent commReadyEvent;
        NSFEvent hardwareResetEvent;

        // Regions and states, from outer to inner
        NSFInitialState initialResetStrategyState;
        NSFCompositeState resetHardwareState;
        NSFCompositeState reestablishCommunicationsState;
        NSFCompositeState readyState;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialResetStrategyToResetHardwareTransition;
        NSFExternalTransition resetHardwareToReestablishCommunicationsTransition;
        NSFExternalTransition reestablishCommunicationsStateToReadyTransition;

        // Actions
        NSFScheduledAction resetHardwareAction;
        NSFScheduledAction readyCommAction;

        void resetHardware(const NSFContext& context);
        void resetComm(const NSFContext& context);

        // State machine guards and actions
        void resetVariables(const NSFStateMachineContext& context);

        bool isHardwareReset(const NSFStateMachineContext& context);
        bool isCommReady(const NSFStateMachineContext& context);

        void resetHardwareEntryActions(const NSFStateMachineContext& context);
        void reestablishCommunicationsEntryActions(const NSFStateMachineContext& context);
    };
}

#endif // RESET_STRATEGY_H