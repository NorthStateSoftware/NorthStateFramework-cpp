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

#ifndef SINGLE_STATE_MACHINE_STRESS_TEST_H
#define SINGLE_STATE_MACHINE_STRESS_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Starts a self triggering state machine.
    /// </summary>
    class ContinuouslyRunningTest : public NSFStateMachine, public ITestInterface
    {
    public:
        ContinuouslyRunningTest(const NSFString& name, int myNumberOfRepeatedEvents);

        ~ContinuouslyRunningTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

        NSFCompositeState* getState1_2_2() { return &state1_2_2;}

    private: 

        int numberOfRepeatedEvents;
        int currentEventCount;

        //Events
        NSFEvent event1;
        NSFEvent event2;
        NSFEvent event3;
        NSFDataEvent<string> event4;
        NSFEvent event5;
        NSFEvent event6;

        // Regions and states, from outer to inner

        //ContinuouslyRunningTest Region
        NSFInitialState test2InitialState;
        NSFCompositeState state1;
        NSFCompositeState state2;

        // State1 Region
        NSFInitialState state1InitialState;
        NSFDeepHistory state1History;
        NSFCompositeState state1_1;
        NSFCompositeState state1_2;

        // State1_2 Region
        NSFInitialState state1_2InitialState;
        NSFCompositeState state1_2_1;
        NSFCompositeState state1_2_2;

        // Transitions, ordered internal, local, external
        // ContinuouslyRunningTest Region
        NSFExternalTransition test1InitialToState1Transition;
        NSFExternalTransition state1ToState2Transition;
        NSFExternalTransition state2ToState1Transition;

        // State1 Region
        NSFExternalTransition state1InitialToState1HistoryTransition;
        NSFExternalTransition state1HistoryToState1_1Transition;
        NSFExternalTransition state1_1ToState1_2Transition;
        NSFExternalTransition state1_2ToState1_1Transition;

        // State1_2 Region
        NSFExternalTransition state1_2InitialStateToState1_2_1Transition;
        NSFExternalTransition state1_2_1ToState1_2_2Transition;
        NSFExternalTransition state1_2_2ToState1_2_1Transition;

        void state1EntryAction(const NSFStateMachineContext& context);
        void state1ExitAction(const NSFStateMachineContext& context);
        void state1_1EntryAction(const NSFStateMachineContext& context);
    };
}

#endif // SINGLE_STATE_MACHINE_STRESS_TEST_H