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

#include "BasicForkJoinTest.h"

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    BasicForkJoinTest::BasicForkJoinTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),
        // Events
        event1("Event1", this),
        event2("Event2", this),

        //States
        // Region A
        regionA("RegionA", this),
        initialAState("InitialA", &regionA),
        stateA1("StateA1", &regionA, NULL, NULL),
        stateA2("StateA2", &regionA, NULL, NULL),

        // Region B
        regionB("RegionB", this),
        initialBState("InitialB", &regionB),
        stateB1("StateB1", &regionB, NULL, NULL),
        stateB2("StateB2", &regionB, NULL, NULL),

        // ForkJoins
        abForkJoin("ABForkJoin", this),

        // Transitions, ordered internal, local, external
        // Region A
        initialAStateToStateA1Transition("InitialAStateToStateA1", &initialAState, &stateA1, NULL, NULL, NULL),
        stateA1ToABForkJoinTransition("StateA1ToABForkJoin", &stateA1, &abForkJoin, &event1, NULL, NULL),
        abForkJoinToStateA2Transition("AbForkJoinToStateA2", &abForkJoin, &stateA2, NULL, NULL, NULL),

        // Region B
        initialBStateToStateB1Transition("InitialBStateToStateB1", &initialBState, &stateB1, NULL, NULL, NULL),
        stateB1ToABForkJoinTransition("StateB1ToABForkJoin", &stateB1, &abForkJoin, &event2, NULL, NULL),
        abForkJoinToStateB2Transition("AbForkJoinToStateB2", &abForkJoin, &stateB2, NULL, NULL, NULL)
    {
    }

    BasicForkJoinTest::~BasicForkJoinTest()
    {
        terminate(true);
        delete getEventThread();
    }

    bool BasicForkJoinTest::runTest(NSFString& errorMessage)
    {
        startStateMachine();

        // Test 
        //  state machine start up 
        //  initial state entering 
        //  null transition.
        //  composite states
        //  user regions
        //  congruent regions
        //  nested states
        //  entry actions
        if (!testHarness.doesEventResultInState(NULL, &stateA1))
        {
            errorMessage = "State Machine did not start properly with congruent states.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  Fork join entry
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &abForkJoin, &stateB1))
        {
            errorMessage = "State Machine did enter the fork join or did not stay in stateB1.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  Fork join entry evaluation
        //  Fork join exit
        //  entry actions
        if (!testHarness.doesEventResultInState(&event2, &stateA2, &stateB2))
        {
            errorMessage = "State Machine did exit the fork join properly.";
            stopStateMachine(); 
            return false;
        }


        stopStateMachine();
        return true;
    }

}
