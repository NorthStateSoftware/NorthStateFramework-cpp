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

#ifndef CYCLE_AB_H
#define CYCLE_AB_H

#include "NorthStateFramework.h"

namespace CycleABExample
{
    class CycleAB : public NSFStateMachine
    {
    public:
        CycleAB(const NSFString& name);

        ~CycleAB();

        // Public interface
        bool isReadyToCycle();
        void startCycle();

    protected: 

        // Local members
        static const int InitializeADelayTime = 100;
        static const int InitializeBDelayTime = 200;
        static const int CompleteADelayTime = 100;
        static const int CompleteBDelayTime = 200;

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        NSFEvent cycleEvent;
        NSFEvent aReadyEvent;
        NSFEvent bReadyEvent;
        NSFEvent aCompleteEvent;
        NSFEvent bCompleteEvent;

        // Regions and states, from outer to inner 
        NSFRegion systemRegion;
        NSFRegion subsystemARegion;
        NSFRegion subsystemBRegion;
        NSFForkJoin initializeForkJoin;
        NSFForkJoin cycleForkJoin;
        NSFForkJoin completeForkJoin;

        // System Region
        // Regions and states, from outer to inner 
        NSFInitialState systemInitialState;
        NSFCompositeState waitForCycleEventState;
        // Transitions, ordered internal, local, external
        NSFExternalTransition systemInitialToInitializeForkJoinTransition;
        NSFExternalTransition initializeForkJoinToWaitForCycleEventTransition;
        NSFExternalTransition waitForCycleEventToCycleForkJoinTransition;
        NSFForkJoinTransition cycleForkJoinToCompleteForkJoinTransiiton;
        NSFExternalTransition completeForkJoinToWaitForCycleEventTransition;

        // Subystem A Region
        // Regions and states, from outer to inner 
        NSFInitialState subsystemAInitialState;
        NSFCompositeState initializeAState;
        NSFCompositeState cycleAState;
        // Transitions, ordered internal, local, external
        NSFExternalTransition subsystemAInitialToInitializeATransition;
        NSFExternalTransition initializeAToInitializeForkJoinTransition;
        NSFForkJoinTransition initializeForkJoinToCycleForkJoinARegionTransition;
        NSFExternalTransition cycleForkJoinToCycleATransition;
        NSFExternalTransition cycleAToCompleteForkJoinTransition;

        // Subystem B Region
        // Regions and states, from outer to inner 
        NSFInitialState subsystemBInitialState;
        NSFCompositeState initializeBState;
        NSFCompositeState cycleBState;
        // Transitions, ordered internal, local, external
        NSFExternalTransition subsystemBInitialToInitializeBTransition;
        NSFExternalTransition initializeBToInitializeForkJoinTransition;
        NSFForkJoinTransition initializeForkJoinToCycleForkJoinBRegionTransition;
        NSFExternalTransition cycleForkJoinToCycleBTransition;
        NSFExternalTransition cycleBToCompleteForkJoinTransition;

        // State machine guards and actions
        void initializeAEntryActions(const NSFStateMachineContext& context);
        void cycleAEntryActions(const NSFStateMachineContext& context);
        void initializeBEntryActions(const NSFStateMachineContext& context);
        void cycleBEntryActions(const NSFStateMachineContext& context);
    };
}

#endif // CYCLE_AB_H