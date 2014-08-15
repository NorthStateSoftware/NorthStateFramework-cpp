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

#include "MultipleTriggersOnTransitionTest.h"

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    MultipleTriggersOnTransitionTest::MultipleTriggersOnTransitionTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),
        event1("Event1", this), 
        event2("Event2", this), 
        event3("Evnet3", this), 
        event4("Event4", this), 
        // Regions and states, from outer to inner
        initialState("InitialTest2", this), 
        state1("State1", this, NULL, NULL), 
        state2("State2", this, NULL, NULL), 
        // Transitions, ordered internal, local, external
        initialAToState1Transition("InitialAToState1", &initialState, &state1, NULL, NULL, NULL), 
        state1ToState2Transition("State1ToState2", &state1, &state2, &event1, NULL, NULL), 
        state2ToState1Transition("State2ToState1", &state2, &state1, &event2, NULL, NULL)
    {
        state1ToState2Transition.addTrigger(&event3);
        state2ToState1Transition.addTrigger(&event4);
    }

    MultipleTriggersOnTransitionTest::~MultipleTriggersOnTransitionTest()
    {
        terminate(true);
        delete getEventThread();
    }

    bool MultipleTriggersOnTransitionTest::runTest(NSFString& errorMessage)
    {
        startStateMachine();

        // Test 
        //  state machine start up 
        //  initial state entering 
        //  null transition.
        //  entry actions
        if (!testHarness.doesEventResultInState(NULL, &state1))
        {
            errorMessage = "State Machine did not start properly.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &state2))
        {
            errorMessage = "State Machine multiple trigger transition failed when event1 did not result in transition from state1 to state2.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event2, &state1))
        {
            errorMessage = "State Machine multiple trigger transition failed when event2 did not result in transition from state2 to state1.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event3, &state2))
        {
            errorMessage = "State Machine multiple trigger transition failed when event3 did not result in transition from state1 to state2.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event4, &state1))
        {
            errorMessage = "State Machine multiple trigger transition failed when event4 did not result in transition from state2 to state1.";
            stopStateMachine(); 
            return false;
        }

        stopStateMachine();
        return true;
    }
}
