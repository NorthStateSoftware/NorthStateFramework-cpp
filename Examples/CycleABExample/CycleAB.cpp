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

#include "CycleAB.h"

namespace CycleABExample
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    CycleAB::CycleAB(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        // Event constructors take the form (name, parent)
        cycleEvent("CycleEvent", this),
        aReadyEvent("AReadyEvent", this),
        bReadyEvent("BReadyEvent", this),
        aCompleteEvent("ACompleteEvent", this),
        bCompleteEvent("BCompleteEvent", this),

        // Regions and states, from outer to inner 
        systemRegion("SystemRegion", this),
        subsystemARegion("SubsystemARegion", this),
        subsystemBRegion("SubsystemBRegion", this),
        initializeForkJoin("InitializeForkJoin", this),
        cycleForkJoin("CycleForkJoin", this),
        completeForkJoin("CompleteForkJoin", this),

        // System Region
        // Regions and states, from outer to inner 
        // Initial state construtors take the form (name, parent)
        systemInitialState("SystemInitial", &systemRegion),
        // Composite state construtors take the form (name, parent, entry actions, exit actions)
        waitForCycleEventState("WaitForCycleEvent", &systemRegion, NULL, NULL),
        // Transitions, ordered internal, local, external
        // External transition construtors take the form (name, source, target, trigger, guard, action)
        systemInitialToInitializeForkJoinTransition("SystemInitialToInitializeForkJoin", &systemInitialState, &initializeForkJoin, NULL, NULL, NULL),
        initializeForkJoinToWaitForCycleEventTransition("InitializeForkJoinToWaitForCycleEvent", &initializeForkJoin, &waitForCycleEventState, NULL, NULL, NULL),
        waitForCycleEventToCycleForkJoinTransition("WaitForCycleEventToCycleForkJoin", &waitForCycleEventState, &cycleForkJoin, &cycleEvent, NULL, NULL),
        cycleForkJoinToCompleteForkJoinTransiiton("CycleForkJoinToCompleteForkJoin", &cycleForkJoin, &completeForkJoin, &systemRegion, NULL),
        completeForkJoinToWaitForCycleEventTransition("CompleteForkJoinToWaitForCycleEvent", &completeForkJoin, &waitForCycleEventState, NULL, NULL, NULL),

        // Subystem A Region
        // Regions and states, from outer to inner 
        // Initial state construtors take the form (name, parent)
        subsystemAInitialState("SubsystemAInitial", &subsystemARegion),
        // Composite state construtors take the form (name, parent, entry actions, exit actions)
        initializeAState("InitializeA", &subsystemARegion, NSFAction(this, &CycleAB::initializeAEntryActions), NULL),
        cycleAState("CycleA", &subsystemARegion, NSFAction(this, &CycleAB::cycleAEntryActions), NULL),
        // Transitions, ordered internal, local, external
        // External transition construtors take the form (name, source, target, trigger, guard, action)
        subsystemAInitialToInitializeATransition("SubsystemAInitialToInitializeA", &subsystemAInitialState, &initializeAState, NULL, NULL, NULL),
        initializeAToInitializeForkJoinTransition("InitializeAToInitializeForkJoin", &initializeAState, &initializeForkJoin, &aReadyEvent, NULL, NULL),
        initializeForkJoinToCycleForkJoinARegionTransition("InitializeForkJoinToCycleForkJoinARegion", &initializeForkJoin, &cycleForkJoin, &subsystemARegion, NULL),
        cycleForkJoinToCycleATransition("CycleForkJoinToCycleA", &cycleForkJoin, &cycleAState, NULL, NULL, NULL),
        cycleAToCompleteForkJoinTransition("CycleAToCompleteForkJoin", &cycleAState, &completeForkJoin, &aCompleteEvent, NULL, NULL),

        // Subystem B Region
        // Regions and states, from outer to inner 
        // Initial state construtors take the form (name, parent)
        subsystemBInitialState("SubsystemBInitial", &subsystemBRegion),
        // Composite state construtors take the form (name, parent, entry actions, exit actions)
        initializeBState("InitializeB", &subsystemBRegion, NSFAction(this, &CycleAB::initializeBEntryActions), NULL),
        cycleBState("CycleB", &subsystemBRegion, NSFAction(this, &CycleAB::cycleBEntryActions), NULL),
        // Transitions, ordered internal, local, external
        // External transition construtors take the form (name, source, target, trigger, guard, action)
        subsystemBInitialToInitializeBTransition("SubsystemBInitialToInitializeB", &subsystemBInitialState, &initializeBState, NULL, NULL, NULL),
        initializeBToInitializeForkJoinTransition("InitializeBToInitializeForkJoin", &initializeBState, &initializeForkJoin, &bReadyEvent, NULL, NULL),
        initializeForkJoinToCycleForkJoinBRegionTransition("InitializeForkJoinToCycleForkJoinBRegion", &initializeForkJoin, &cycleForkJoin, &subsystemBRegion, NULL),
        cycleForkJoinToCycleBTransition("CycleForkJoinToCycleB", &cycleForkJoin, &cycleBState, NULL, NULL, NULL),
        cycleBToCompleteForkJoinTransition("CycleBToCompleteForkJoin", &cycleBState, &completeForkJoin, &bCompleteEvent, NULL, NULL)
    {
    }

    CycleAB::~CycleAB()
    {
        // It is good practice to stop event processing (terminate) in the 
        // destructor of a state machine or event handler to prevent 
        // entry/exit actions from being performed on a destructing object.
        terminate(true);
        delete getEventThread();
    }

    bool CycleAB::isReadyToCycle()
    {
        if (!waitForCycleEventState.isActive())
        {
            return false;
        }

        if (hasEvent(&cycleEvent))  
        {
            return false;
        }

        return true;
    }

    void CycleAB::startCycle()
    {
        queueEvent(&cycleEvent);
    }

    void CycleAB::initializeAEntryActions(const NSFStateMachineContext&)
    {
        aReadyEvent.schedule(InitializeADelayTime, 0);
    }

    void CycleAB::cycleAEntryActions(const NSFStateMachineContext&)
    {
        aCompleteEvent.schedule(CompleteADelayTime, 0);
    }

    void CycleAB::initializeBEntryActions(const NSFStateMachineContext&)
    {
        bReadyEvent.schedule(InitializeBDelayTime, 0);
    }

    void CycleAB::cycleBEntryActions(const NSFStateMachineContext&)
    {
        bCompleteEvent.schedule(CompleteBDelayTime, 0);
    }

}
