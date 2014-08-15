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

#include "TransitionOrderTest.h"

using namespace NorthStateFramework;

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    TransitionOrderTest::TransitionOrderTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),
        // Events
        event1("Event1", this),
        event2("Event2", this),
        event3("Event3", this),

        // Regions and states, from outer to inner
        initialTest15("InitialTest16", this),
        state1("State1", this, NULL, NULL),
        state2("State2", this, NULL, NULL),
        intialState2("IntialState2", &state2),
        state2_1("State2_1", &state2, NULL, NSFAction(this, &TransitionOrderTest::state2_1ExitActions)),
        state3("State3", this, NULL, NULL),
        initialState3("InitialState3", &state3),
        state3_1("State3_1",  &state3, NULL, NSFAction(this, &TransitionOrderTest::state3_1ExitActions)),
        state4("State4", this, NULL, NULL),
        errorState("Error", this, NULL, NULL),

        // Transitions, ordered internal, local, external
        initialTest15ToState1Transition("InitialTest15ToState1", &initialTest15, &state1, NULL, NULL, NULL),
        state1ToState2Transition("State1ToState2", &state1, &state2, &event2, NULL, NULL),
        state1ReactionToEvent1("State1ReactionToEvent1", &state1, &event1, NULL, NSFAction(this, &TransitionOrderTest::state1ReactionToEvent1Actions)),
        state1ToErrorTransition("State1ToError", &state1, &errorState, &event1, NULL, NULL),

        state2ToState3Transition("State2ToState3Transition", &state2, &state3, &event2, NULL, NULL),
        state2ToErrorTransition("State2ToErrorTransition", &state2, &errorState, &event1, NULL, NULL),
        state2ToState2Transition("State2ToState2Transition", &state2, &state2, &event1, NULL, NULL),
        intialState2ToState2_1Transition("intialState2ToState2_1Transition", &intialState2, &state2_1, NULL, NULL, NULL),

        state3ToState4Transition("State3ToState4Transition", &state3, &state4, &event2, NULL, NULL),
        state3ToState3Transition("State3ToState3Transition", &state3, &state3, &event1, NULL, NULL),
        state3ReactionToEvent1("state3ReactionToEvent1", &state3, &event1, NULL, NSFAction(this, &TransitionOrderTest::state3ReactionToEvent1Actions)),
        initialState3ToState3_1Transition("InitialState3ToState3_1Transition", &initialState3, &state3_1, NULL, NULL, NULL),
        state3ToErrorTransition("State3ToErrorTransition", &state3, &errorState, &event3, NULL, NULL)
    {
    }

    TransitionOrderTest::~TransitionOrderTest()
    {
        terminate(true);
        delete getEventThread();
    }

    bool TransitionOrderTest::runTest(NSFString& errorMessage)
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
        //  that internal transitions are taken before external transitions
        //  internal tranision actions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &state2))
        {
            errorMessage = "Internal transition was not taken before external transition.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  that local transitions are taken before external transitions
        //  that local tranisition properly exit sub states
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &state3))
        {
            errorMessage = "Local transition was not taken before external transition.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  that internal transitions are taken before local transitions
        //  internal tranision actions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &state4))
        {
            errorMessage = "Internal transition was not taken before local transition.";
            stopStateMachine(); 
            return false;
        }

        stopStateMachine();
        return true;
    }

    void TransitionOrderTest::state1ReactionToEvent1Actions(const NSFStateMachineContext& context)
    {
        queueEvent(&event2);
    }

    void TransitionOrderTest::state3ReactionToEvent1Actions(const NSFStateMachineContext& context)
    {
        queueEvent(&event2);
    }

    void TransitionOrderTest::state2_1ExitActions(const NSFStateMachineContext& context)
    {
        queueEvent(&event2);
    }

    void TransitionOrderTest::state3_1ExitActions(const NSFStateMachineContext& context)
    {
        queueEvent(&event3);
    }
}