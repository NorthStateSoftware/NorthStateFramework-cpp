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

#ifndef BASIC_FORK_JOIN_TEST_H
#define BASIC_FORK_JOIN_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

namespace NSFTest
{
    /// <summary>
    /// Test Fork Join basic functionality.
    /// </summary>
    class BasicForkJoinTest : public NSFStateMachine, public ITestInterface
    {
    public:
        BasicForkJoinTest(const NSFString& name);

        ~BasicForkJoinTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private: 

        TestHarness testHarness;

        //Events
        NSFEvent event1;
        NSFEvent event2;

        // Regions and states, from outer to inner
        // Region A
        NSFRegion regionA;
        NSFInitialState initialAState;
        NSFCompositeState stateA1;
        NSFCompositeState stateA2;

        // Region B
        NSFRegion regionB;
        NSFInitialState initialBState;
        NSFCompositeState stateB1;
        NSFCompositeState stateB2;

        // ForkJoins
        NSFForkJoin abForkJoin;

        // Transitions, ordered internal, local, external
        // Region A
        NSFExternalTransition initialAStateToStateA1Transition;
        NSFExternalTransition stateA1ToABForkJoinTransition;
        NSFExternalTransition abForkJoinToStateA2Transition;

        // Region B
        NSFExternalTransition initialBStateToStateB1Transition;
        NSFExternalTransition stateB1ToABForkJoinTransition;
        NSFExternalTransition abForkJoinToStateB2Transition;
    };
}

#endif // BASIC_FORK_JOIN_TEST_H