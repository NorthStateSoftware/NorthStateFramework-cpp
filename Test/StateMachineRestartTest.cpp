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

#include "StateMachineRestartTest.h"

using namespace NorthStateFramework;

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    StateMachineRestartTest::StateMachineRestartTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),
        // Events
        event1("Event1", this),
        event2("Event2", this),

        // Regions and states, from outer to inner
        initialState("InitialTest10", this),
        state1("State1", this, NULL, NULL),
        state2("State2", this, NULL, NULL),
        state3("State3", this, NULL, NULL),

        // Transitions, ordered internal, local, external
        test1ReactionToEvent2("Test1ReactionToEvent2", this, &event2, NULL, NSFAction(this, &StateMachineRestartTest::test1ReactionToEvent2Actions)),
        state2ReactionToEvent1("State2ReactionToEvent1", &state2, &event1, NULL, NSFAction(this, &StateMachineRestartTest::state2ReactionToEvent1Actions)),
        initialToState1Transition("InitialToState1", &initialState, &state1, NULL, NULL, NULL),
        state1ToState2Transition("State1ToState2", &state1, &state2, &event1, NULL, NULL),
        state2ToState3Transition("State2ToState3", &state2, &state3, &event2, NULL, NULL),
        state3ToState2Transition("State3ToState2", &state3, &state2, &event1, NULL, NULL),
        state2ToState1Transition("State2ToState1", &state2, &state1, &event1, NULL, NULL)
    {
    }

    StateMachineRestartTest::~StateMachineRestartTest()
    {
        terminate(true);
        delete getEventThread();
    }

    bool StateMachineRestartTest::runTest(NSFString& errorMessage)
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
            errorMessage = "State Machine did not handle simple event triggered transition from state1 to state2";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event2, &state3))
        {
            errorMessage = "State Machine did not handle simple event triggered transition from state2 to state3";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &state2))
        {
            errorMessage = "State Machine did not handle simple event triggered transition from state3 to state2";
            stopStateMachine(); 
            return false;
        }

        resetStateMachine();

        // Test 
        //  state machine restart
        //  initial state entering 
        //  null transition.
        //  entry actions
        if (!testHarness.doesEventResultInState(NULL, &state1))
        {
            errorMessage = "State Machine did not reset properly.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &state2))
        {
            errorMessage = "State Machine did not handle simple event triggered transition from state1 to state2 after reset";
            stopStateMachine(); 
            return false;
        }

        stopStateMachine();
        return true;
    }

    void StateMachineRestartTest::state2ReactionToEvent1Actions(const NSFStateMachineContext&)
    {
        queueEvent(&event2);
    }

    void StateMachineRestartTest::test1ReactionToEvent2Actions(const NSFStateMachineContext&)
    {
        queueEvent(&event1);
    }
}
