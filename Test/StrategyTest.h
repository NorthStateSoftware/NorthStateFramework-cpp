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

#ifndef STRATEGY_TEST_H
#define STRATEGY_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    class State2Strategy : public NSFStateMachine
    {
    public:

        State2Strategy(const NSFString& name, NSFCompositeState* parentState);
        ~State2Strategy();

        NSFEvent& getEvent3() { return event3; }
        NSFState& getState4() { return state4; }
        NSFState& getState5() { return state5; }

    private:

        // Events
        NSFEvent event3;

        // Regions and states, from outer to inner
        NSFInitialState intialState2;
        NSFCompositeState state4;
        NSFCompositeState state5;

        // Transitions, ordered internal, local, external
        NSFExternalTransition intialState2ToState4Transition;
        NSFExternalTransition state4ToState5Transition;
    };

    /// <summary>
    /// Test using state machine as substates.
    /// </summary>
    class StrategyTest : public NSFStateMachine, public ITestInterface
    {
    public:
        StrategyTest(const NSFString& name);

        ~StrategyTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

        void state5EntryActions(const NSFStateMachineContext& context);

    private:

        TestHarness testHarness;

        //Events
        NSFEvent event1;
        NSFEvent event2;

        // Regions and states, from outer to inner
        NSFInitialState initialState;
        NSFCompositeState state1;
        NSFCompositeState state2;
        NSFCompositeState state3;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialToState1Transition;
        NSFExternalTransition state1ToState2Transition;
        NSFExternalTransition state2ToState3Transition;
        NSFExternalTransition state3ToState2Transition;
        NSFExternalTransition state2ToState1Transition;

        State2Strategy state2Strategy;
    };
}

#endif // STRATEGY_TEST_H