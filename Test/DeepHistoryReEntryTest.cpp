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

#include "DeepHistoryReEntryTest.h"

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    DeepHistoryReEntryTest::DeepHistoryReEntryTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),

        // Events
        event1("Event1", this),
        event2("Event2", this),
        event3("Event3", this),
        event4("Event4", this),
        event5("Event5", this),
        event6("Event6", this),
        event7("Event7", this),
        event8("Event8", this),

        //States
        //DeepHistoryReEntryTest Region
        test2InitialState("InitialTest8", this),
        state1("State1", this, NULL, NULL),
        state2("State2", this, NULL, NULL),

        //State 1 Region
        state1InitialState("InitialState1", &state1),
        state1History("State1History", &state1),
        state1_1("State1_1", &state1, NULL, NULL),
        state1_2("State1_2", &state1, NULL, NULL),

        // State1_2 Region
        state1_2InitialState("InitialState1_2", &state1_2),
        state1_2_1("State1_2_1", &state1_2, NULL, NULL),
        state1_2_2("State1_2_2", &state1_2, NULL, NULL),

        //Transitions
        // DeepHistoryReEntryTest Region
        test1InitialToState1Transition("Test1InitialToState1", &test2InitialState, &state1, NULL, NULL, NULL),
        state1ToState2Transition("State1ToState2", &state1, &state2, &event5, NULL, NULL),
        state2ToState1Transition("State2ToState1", &state2, &state1, &event6, NULL, NULL),
        state1_2_2ToState2Transition("State1_2_2ToState2", &state1_2_2, &state2, &event7, NULL, NULL),
        state2Tostate1HistoryTransition("State2Tostate1History", &state2, &state1History, &event8, NULL, NULL),

        // &state1 Region
        state1InitialToState1_1Transition("State1InitialToState1_1", &state1InitialState, &state1_1, NULL, NULL, NULL),
        state1HistoryToState1_1Transition("State1HistoryToState1_1", &state1History, &state1_1, NULL, NULL, NULL),
        state1_1ToState1_2Transition("State1_1ToState1_2", &state1_1, &state1_2, &event1, NULL, NULL),
        state1_2ToState1_1Transition("State1_2ToState1_1Transition ", &state1_2, &state1_1, &event2, NULL, NULL),

        // &state1_2 Region
        state1_2InitialStateToState1_2_1Transition("State1_2InitialStateToState1_2_1", &state1_2InitialState, &state1_2_1, NULL, NULL, NULL),
        state1_2_1ToState1_2_2Transition("State1_2_1ToState1_2_2", &state1_2_1, &state1_2_2, &event3, NULL, NULL),
        state1_2_2ToState1_2_1Transition("State1_2_2ToState1_2_1", &state1_2_2, &state1_2_1, &event4, NULL, NULL)
    {
    }

    DeepHistoryReEntryTest::~DeepHistoryReEntryTest()
    {
        terminate(true);
        delete getEventThread();
    }

    bool DeepHistoryReEntryTest::runTest(NSFString& errorMessage)
    {
        startStateMachine();

        // Test 
        //  state machine start up 
        //  initial state entering 
        //  null transition.
        //  composite states
        //  default regions
        //  nested states
        //  entry actions
        if (!testHarness.doesEventResultInState(NULL, &state1_1))
        {
            errorMessage = "State Machine did not start properly.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  initial state entering 
        //  null transition.
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &state1_2_1))
        {
            errorMessage = "State Machine did not handle simple event triggered transition from state1_1 to state1_2_1";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event3, &state1_2_2))
        {
            errorMessage = "State Machine did not handle simple event triggered transition from state1_2_1 to state1_2_2.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event5, &state2))
        {
            errorMessage = "State Machine did not handle simple event triggered transition from state1_2_2 to state2.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  deep history direct entry
        //  entry actions
        if (!testHarness.doesEventResultInState(&event8, &state1_2_2))
        {
            errorMessage = "State Machine did not handle transitioning into the deep history properly.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(&event7, &state2))
        {
            errorMessage = "State Machine did not handle simple event triggered transition from state1_2_2 to state2 after history.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  deep history direct entry
        //  deep history re-entry
        //  entry actions
        if (!testHarness.doesEventResultInState(&event8, &state1_2_2))
        {
            errorMessage = "State Machine did not handle re-entry into the deep history properly.";
            stopStateMachine(); 
            return false;
        }

        stopStateMachine();
        return true;
    }

}
