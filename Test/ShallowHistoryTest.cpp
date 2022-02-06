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

#include "ShallowHistoryTest.h"

using namespace NorthStateFramework;

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    //#region Constructors
    ShallowHistoryTest::ShallowHistoryTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),
        // Events
        event1("Event1", this),
        event2("Event2", this),
        event3("Event3", this),
        event4("Event4", this),
        event5("Event5", this),
        event6("Event6", this),
        //States
        test2InitialState("InitialTest2", this),
        state1("State1", this, NULL, NULL),
        state2("State2", this, NULL, NULL),
        //State 1 Region
        state1InitialState("State1Initial", &state1),
        state1History("State1History", &state1),
        state1_1("State1_1", &state1, NULL, NULL),
        state1_2("State1_2", &state1, NULL, NULL),
        // State1_2 Region
        state1_2InitialState("State1_2Initial", &state1_2),
        state1_2_1("State1_2_1", &state1_2, NULL, NULL),
        state1_2_2("State1_2_2", &state1_2, NULL, NULL),
        //Transitions
        // ShallowHistoryTest Region
        test1InitialToState1Transition("Test1InitialToState1", &test2InitialState, &state1, NULL, NULL, NULL),
        state1ToState2Transition("State1ToState2", &state1, &state2, &event5, NULL, NULL),
        state2ToState1Transition("State2ToState1", &state2, &state1, &event6, NULL, NULL),
        // State1 Region
        state1InitialToState1HistoryTransition("State1InitialToState1History", &state1InitialState, &state1History, NULL, NULL, NULL),
        state1HistoryToState1_1Transition("State1HistoryToState1_1", &state1History, &state1_1, NULL, NULL, NULL),
        state1_1ToState1_2Transition("State1_1ToState1_2", &state1_1, &state1_2, &event1, NULL, NULL),
        state1_2ToState1_1Transition("State1_2ToState1_1", &state1_2, &state1_1, &event2, NULL, NULL),
        // State1_2 Region
        state1_2InitialStateToState1_2_1Transition("State1_2InitialStateToState1_2_1", &state1_2InitialState, &state1_2_1, NULL, NULL, NULL),
        state1_2_1ToState1_2_2Transition("State1_2_1ToState1_2_2", &state1_2_1, &state1_2_2, &event3, NULL, NULL),
        state1_2_2ToState1_2_1Transition("State1_2_2ToState1_2_1", &state1_2_2, &state1_2_1, &event4, NULL, NULL)
    {
    }

    ShallowHistoryTest::~ShallowHistoryTest()
    {
        terminate(true);
        delete getEventThread();
    }


    bool ShallowHistoryTest::runTest(NSFString& errorMessage)
    {
        startStateMachine();

        // Test 
        //  state machine start up 
        //  initial state entering 
        //  null transition.
        //  composite states
        //  default regions
        //  nested states
        //  History connectors default transitions
        //  entry actions
        if (!testHarness.doesEventResultInState(NULL, &state1_1))
        {
            errorMessage = "State Machine did not start properly when going through a shallow history state in startup.";
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
            errorMessage = "State Machine did not handle simple event triggered transition from state1_1 to state1_2_1.";
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
        //  Shallow history 
        //  entry actions
        if (!testHarness.doesEventResultInState(&event6, &state1_2_1))
        {
            errorMessage = "State Machine did not handle transitioning through the shallow history properly.";
            stopStateMachine(); 
            return false;
        }

        stopStateMachine();
        return true;
    }
}
