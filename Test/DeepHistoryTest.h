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

#ifndef DEEP_HISTORY_TEST_H
#define DEEP_HISTORY_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test deep history
    /// </summary>
    class DeepHistoryTest : public NSFStateMachine, public ITestInterface
    {
    public:
        DeepHistoryTest(const NSFString& name);

        ~DeepHistoryTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private: 

        TestHarness testHarness;

        //Events
        NSFEvent event1;
        NSFEvent event2;
        NSFEvent event3;
        NSFEvent event4;
        NSFEvent event5;
        NSFEvent event6;

        // Regions and states, from outer to inner

        //DeepHistoryTest Region
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
        // DeepHistoryTest Region
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
    };
}

#endif // DEEP_HISTORY_TEST_H