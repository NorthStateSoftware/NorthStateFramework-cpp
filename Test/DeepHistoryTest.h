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

#ifndef DEEP_HISTORY_TEST_H
#define DEEP_HISTORY_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test deep history
    /// </summary>
    class DeepHistoryTest : public NSFStateMachine, public ITestInterface
    {
    public:
        DeepHistoryTest(const NSFString& name);

        ~DeepHistoryTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private: 

        TestHarness testHarness;

        //Events
        NSFEvent event1;
        NSFEvent event2;
        NSFEvent event3;
        NSFEvent event4;
        NSFEvent event5;
        NSFEvent event6;

        // Regions and states, from outer to inner

        //DeepHistoryTest Region
        NSFInitialState test2InitialState;
        NSFCompositeState state1;
        NSFCompositeState state2;

        // State1 Region
        NSFInitialState state1InitialState;
        NSFDeepHistory state1History;
        NSFCompositeState state1_1;
        NSFCompositeState state1_2;

        // State1_2 Region
        NSFInitialState state1_2InitialState;
        NSFCompositeState state1_2_1;
        NSFCompositeState state1_2_2;

        // Transitions, ordered internal, local, external
        // DeepHistoryTest Region
        NSFExternalTransition test1InitialToState1Transition;
        NSFExternalTransition state1ToState2Transition;
        NSFExternalTransition state2ToState1Transition;

        // State1 Region
        NSFExternalTransition state1InitialToState1HistoryTransition;
        NSFExternalTransition state1HistoryToState1_1Transition;
        NSFExternalTransition state1_1ToState1_2Transition;
        NSFExternalTransition state1_2ToState1_1Transition;

        // State1_2 Region
        NSFExternalTransition state1_2InitialStateToState1_2_1Transition;
        NSFExternalTransition state1_2_1ToState1_2_2Transition;
        NSFExternalTransition state1_2_2ToState1_2_1Transition;
    };
}

#endif // DEEP_HISTORY_TEST_H