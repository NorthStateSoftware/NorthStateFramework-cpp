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

#ifndef FORK_JOIN_TO_FORK_JOIN_TRANSITION_TEST_H
#define FORK_JOIN_TO_FORK_JOIN_TRANSITION_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test Fork Join to Fork Join Transitions
    /// Extra regional transitions to ForkJoins
    /// </summary>
    class ForkJoinToForkJoinTransitionTest : public NSFStateMachine, public ITestInterface
    {
    public:
        ForkJoinToForkJoinTransitionTest(const NSFString& name);

        ~ForkJoinToForkJoinTransitionTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private:

        TestHarness testHarness;

        // Fork Joins
        NSFForkJoin bSynch;
        NSFForkJoin abForkJoin;
        NSFForkJoin bcForkJoin;

        // ARegion
        // Events
        NSFEvent evA1;

        // Regions and states, from outer to inner
        NSFRegion aRegion;
        NSFInitialState initialAState;
        NSFCompositeState stateA1;
        NSFCompositeState stateA2;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialAToStateA1Transition;
        NSFExternalTransition stateA1ToABForkJoinTransition;
        NSFExternalTransition abForkJoinToA2Transition;

        // B Region
        // Events
        NSFEvent evB1;

        // Regions and states, from outer to inner
        NSFRegion bRegion;
        NSFInitialState initialBState;
        NSFCompositeState stateB1;
        NSFCompositeState stateB2;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialBToStateB1Transition;
        NSFExternalTransition stateB1ToBSynchTransition;
        NSFExternalTransition bSynchToStateB2Transition;

        // C Region
        // Events
        NSFEvent evC1;

        // Regions and states, from outer to inner
        NSFRegion cRegion;
        NSFInitialState initialCState;
        NSFCompositeState stateC1;
        NSFCompositeState stateC2;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialCToStateC1Transition;
        NSFExternalTransition stateC1ToBCForkJoinTransition;
        NSFExternalTransition bcForkJoinToC2Transition;

        // Extra Regional Transitions
        NSFExternalTransition bSynchToABForkJoinTransition;
        NSFExternalTransition bSynchToBCForkJoinTransition;
    };
}

#endif // FORK_JOIN_TO_FORK_JOIN_TRANSITION_TEST_H