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

#ifndef RESET_STRATEGY_H
#define RESET_STRATEGY_H

#include "NorthStateFramework.h"

using namespace std;

namespace CommandProcessorWithResetStrategyExample
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