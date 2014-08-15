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

#ifndef TRANSITION_ORDER_TEST_H
#define TRANSITION_ORDER_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test if the event consumption order for internal, local, and external transitions.
    /// </summary>
    class TransitionOrderTest : public NSFStateMachine, public ITestInterface
    {
    public:
        TransitionOrderTest(const NSFString& name);

        ~TransitionOrderTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private:

        TestHarness testHarness;

        //Events
        NSFEvent event1;
        NSFEvent event2;
        NSFEvent event3;

        // Regions and states, from outer to inner
        NSFInitialState initialTest15;
        NSFCompositeState state1;
        NSFCompositeState state2;
        NSFInitialState intialState2;
        NSFCompositeState state2_1;
        NSFCompositeState state3;
        NSFInitialState initialState3;
        NSFCompositeState state3_1;
        NSFCompositeState state4;
        NSFCompositeState errorState;

        // Transitions, ordered internal, local, external
        // Intentionally mis-ordered
        NSFExternalTransition initialTest15ToState1Transition;
        NSFExternalTransition state1ToState2Transition;
        NSFInternalTransition state1ReactionToEvent1;
        NSFExternalTransition state1ToErrorTransition;

        NSFExternalTransition state2ToState3Transition;
        NSFExternalTransition state2ToErrorTransition;
        NSFLocalTransition state2ToState2Transition;
        NSFExternalTransition intialState2ToState2_1Transition;

        NSFExternalTransition state3ToState4Transition;
        NSFLocalTransition state3ToState3Transition;
        NSFInternalTransition state3ReactionToEvent1;
        NSFExternalTransition initialState3ToState3_1Transition;
        NSFExternalTransition state3ToErrorTransition;

        void state1ReactionToEvent1Actions(const NSFStateMachineContext& context);
        void state3ReactionToEvent1Actions(const NSFStateMachineContext& context);

        void state2_1ExitActions(const NSFStateMachineContext& context);
        void state3_1ExitActions(const NSFStateMachineContext& context);
    };
}

#endif // TRANSITION_ORDER_TEST_H