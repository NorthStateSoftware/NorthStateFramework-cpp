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

#include "ResetStrategy.h"

namespace CommandProcessorWithResetStrategyExample
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    ResetStrategy::ResetStrategy(const NSFString& name, NSFCompositeState* parentState)
        : NSFStateMachine(name, parentState),  hardwareReset(false), commReady(false), hardwareResetDelayTime(1000), commReadyDelayTime(1000),

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        commReadyEvent("CommReady", this),
        hardwareResetEvent("HardwareReset", this),

        // Regions and states, from outer to inner 
        // Initial state construtors take the form (name, parent)
        initialResetStrategyState("InitialResetStrategy", this),
        // Composite state construtors take the form (name, parent, entry action, exit action)
        resetHardwareState("ResetHardware", this, NSFAction(this, &ResetStrategy::resetHardwareEntryActions), NULL),
        reestablishCommunicationsState("ReestablishCommunications", this, NSFAction(this, &ResetStrategy::reestablishCommunicationsEntryActions), NULL),
        readyState("Ready", this, NULL, NULL),

        // Transitions, ordered internal, local, external
        // External transition construtors take the form (name, source, target, trigger, guard, action)
        initialResetStrategyToResetHardwareTransition("InitialToResetHardware", &initialResetStrategyState, &resetHardwareState, NULL, NULL, NSFAction(this, &ResetStrategy::resetVariables)),        
        resetHardwareToReestablishCommunicationsTransition("ResetHardwareToReestablishCommunications", &resetHardwareState, &reestablishCommunicationsState, NULL, NSFGuard(this, &ResetStrategy::isHardwareReset), NULL),
        reestablishCommunicationsStateToReadyTransition("ReestablishCommunicationsStateToReady", &reestablishCommunicationsState, &readyState, NULL, NSFGuard(this, &ResetStrategy::isCommReady), NULL),

        // Actions
        resetHardwareAction("ReadHardware", NSFAction(this, &ResetStrategy::resetHardware), getEventThread()),
        readyCommAction("ResetComm", NSFAction(this, &ResetStrategy::resetComm), getEventThread())
    {
    }

    ResetStrategy::~ResetStrategy()
    {
        // It is good practice to stop event processing (terminate) in the 
        // destructor of a state machine or event handler to prevent 
        // entry/exit actions from being performed on a destructing object.
        terminate(true);
    }

    void ResetStrategy::resetVariables(const NSFStateMachineContext&)
    {
        hardwareReset = false;
        commReady = false;
    }

    void ResetStrategy::resetHardware(const NSFContext&)
    {
        hardwareReset = true;
        queueEvent(&hardwareResetEvent);
    }

    void ResetStrategy::resetComm(const NSFContext&)
    {
        commReady = true;
        queueEvent(&commReadyEvent);
    }

    void ResetStrategy::resetHardwareEntryActions(const NSFStateMachineContext&)
    {
        hardwareReset = false;

        // Simulate behavior using an action
        resetHardwareAction.schedule(hardwareResetDelayTime);
    }

    void ResetStrategy::reestablishCommunicationsEntryActions(const NSFStateMachineContext&)
    {
        commReady = false;

        // Simulate behavior using an action
        readyCommAction.schedule(commReadyDelayTime);
    }

    bool ResetStrategy::isHardwareReset(const NSFStateMachineContext&)
    {
        return hardwareReset;
    }

    bool ResetStrategy::isCommReady(const NSFStateMachineContext&)
    {
        return commReady;
    }
}
