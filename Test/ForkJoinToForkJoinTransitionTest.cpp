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

#include "ForkJoinToForkJoinTransitionTest.h"

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    ForkJoinToForkJoinTransitionTest::ForkJoinToForkJoinTransitionTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),
        // Fork Joins
        bSynch("BSynch", this),
        abForkJoin("ABForkJoin", this),
        bcForkJoin("BCForkJoin", this),
        // ARegion
        // Events
        evA1("EvA1", this),
        // States
        aRegion("ARegion", this),
        initialAState("InitialA", &aRegion),
        stateA1("StateA1", &aRegion, NULL, NULL),
        stateA2("StateA2", &aRegion, NULL, NULL),
        // Transitions, ordered internal, local, external
        initialAToStateA1Transition("InitialAToStateA1", &initialAState, &stateA1, NULL, NULL, NULL),
        stateA1ToABForkJoinTransition("StateA1ToABForkJoin", &stateA1, &abForkJoin, &evA1, NULL, NULL),
        abForkJoinToA2Transition("ABForkJoinToA2", &abForkJoin, &stateA2, NULL, NULL, NULL),
        // B Region
        // Events
        evB1("EvB1", this),
        // States
        bRegion("BRegion", this),
        initialBState("InitialB", &bRegion),
        stateB1("StateB1", &bRegion, NULL, NULL),
        stateB2("StateB2", &bRegion, NULL, NULL),
        // Transitions, ordered internal, local, external
        initialBToStateB1Transition("InitialBToStateB1", &initialBState, &stateB1, NULL, NULL, NULL),
        stateB1ToBSynchTransition("StateB1ToBSynch", &stateB1, &bSynch, &evB1, NULL, NULL),
        bSynchToStateB2Transition("BSynchToStateB2", &bSynch, &stateB2, NULL, NULL, NULL),
        // C Region
        // Events
        evC1("EvC1", this),
        // States
        cRegion("CRegion", this),
        initialCState("InitialC", &cRegion),
        stateC1("StateC1", &cRegion, NULL, NULL),
        stateC2("StateC2", &cRegion, NULL, NULL),
        // Transitions, ordered internal, local, external
        initialCToStateC1Transition("InitialCToStateC1", &initialCState, &stateC1, NULL, NULL, NULL),
        stateC1ToBCForkJoinTransition("StateC1ToBCForkJoin", &stateC1, &bcForkJoin, &evC1, NULL, NULL),
        bcForkJoinToC2Transition("BCForkJoinToC2", &bcForkJoin, &stateC2, NULL, NULL, NULL),

        // Extra Regional Transitions
        bSynchToABForkJoinTransition("BSynchToABForkJoin", &bSynch, &abForkJoin, NULL, NULL, NULL),
        bSynchToBCForkJoinTransition("BSynchToBCForkJoin", &bSynch, &bcForkJoin, NULL, NULL, NULL)

    {
    }

    ForkJoinToForkJoinTransitionTest::~ForkJoinToForkJoinTransitionTest()
    {
        terminate(true);
        delete getEventThread();
    }

    bool ForkJoinToForkJoinTransitionTest::runTest(NSFString& errorMessage)
    {
        startStateMachine();

        // Test 
        //  state machine start up 
        //  initial state entering 
        //  null transition.
        //  composite states
        //  user regions
        //  congruent regions
        //  entry actions
        if (!testHarness.doesEventResultInState(NULL, &stateA1, &stateC1))
        {
            errorMessage = "State Machine did not start properly into stateA1 and stateC1.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  Fork join entry
        //  entry actions
        if (!testHarness.doesEventResultInState(&evA1, &abForkJoin))
        {
            errorMessage = "State Machine did enter the fork join abForkJoin.";
            stopStateMachine(); 
            return false;
        }

        // Test
        //  That the fork join does not spontaneously exit
        if (testHarness.doesEventResultInState(NULL, &stateA2))
        {
            errorMessage = "State Machine entered stateA2 and should not have until after all entry transitions had been taken.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  Fork join transitions
        //  Fork join entry evaluation
        //  Fork join exit
        //  entry actions
        if (!testHarness.doesEventResultInState(&evB1, &stateA2, &stateB2))
        {
            errorMessage = "State Machine did not handle the fork join to fork join transition properly.";
            stopStateMachine(); 
            return false;
        }

        // Test
        //  That the fork join does not spontaneously exit
        if (testHarness.doesEventResultInState(NULL, &stateC2))
        {
            errorMessage = "State Machine entered stateC2 and should not have until after all entry transitions had been taken.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  Fork join transitions
        //  Fork join entry evaluation
        //  Fork join exit
        //  entry actions
        if (!testHarness.doesEventResultInState(&evC1, &stateC2))
        {
            errorMessage = "State Machine did not handle the fork join to fork join transition properly.";
            stopStateMachine(); 
            return false;
        }

        stopStateMachine();
        return true;
    }
}
