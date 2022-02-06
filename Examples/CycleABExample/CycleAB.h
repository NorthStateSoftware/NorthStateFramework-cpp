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