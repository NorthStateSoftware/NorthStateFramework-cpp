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

#ifndef TRANSITION_ORDER_TEST_H
#define TRANSITION_ORDER_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test if the event consumption order for internal, local, and external transitions.
    /// </summary>
    class TransitionOrderTest : public NSFStateMachine, public ITestInterface
    {
    public:
        TransitionOrderTest(const NSFString& name);

        ~TransitionOrderTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private:

        TestHarness testHarness;

        //Events
        NSFEvent event1;
        NSFEvent event2;
        NSFEvent event3;

        // Regions and states, from outer to inner
        NSFInitialState initialTest15;
        NSFCompositeState state1;
        NSFCompositeState state2;
        NSFInitialState intialState2;
        NSFCompositeState state2_1;
        NSFCompositeState state3;
        NSFInitialState initialState3;
        NSFCompositeState state3_1;
        NSFCompositeState state4;
        NSFCompositeState errorState;

        // Transitions, ordered internal, local, external
        // Intentionally mis-ordered
        NSFExternalTransition initialTest15ToState1Transition;
        NSFExternalTransition state1ToState2Transition;
        NSFInternalTransition state1ReactionToEvent1;
        NSFExternalTransition state1ToErrorTransition;

        NSFExternalTransition state2ToState3Transition;
        NSFExternalTransition state2ToErrorTransition;
        NSFLocalTransition state2ToState2Transition;
        NSFExternalTransition intialState2ToState2_1Transition;

        NSFExternalTransition state3ToState4Transition;
        NSFLocalTransition state3ToState3Transition;
        NSFInternalTransition state3ReactionToEvent1;
        NSFExternalTransition initialState3ToState3_1Transition;
        NSFExternalTransition state3ToErrorTransition;

        void state1ReactionToEvent1Actions(const NSFStateMachineContext& context);
        void state3ReactionToEvent1Actions(const NSFStateMachineContext& context);

        void state2_1ExitActions(const NSFStateMachineContext& context);
        void state3_1ExitActions(const NSFStateMachineContext& context);
    };
}

#endif // TRANSITION_ORDER_TEST_H