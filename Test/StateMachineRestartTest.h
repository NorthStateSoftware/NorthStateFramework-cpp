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

#ifndef STATE_MACHINE_RESTART_TEST_H
#define STATE_MACHINE_RESTART_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test State Machine Restart
    /// </summary>
    class StateMachineRestartTest : public NSFStateMachine, public ITestInterface
    {
    public:
        StateMachineRestartTest(const NSFString& name);

        ~StateMachineRestartTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private:

        TestHarness testHarness;

        //Events
        NSFEvent event1;
        NSFEvent event2;

        // Regions and states, from outer to inner
        NSFInitialState initialState;
        NSFCompositeState state1;
        NSFCompositeState state2;
        NSFCompositeState state3;

        // Transitions, ordered internal, local, external
        NSFInternalTransition test1ReactionToEvent2;
        NSFInternalTransition state2ReactionToEvent1;
        NSFExternalTransition initialToState1Transition;
        NSFExternalTransition state1ToState2Transition;
        NSFExternalTransition state2ToState3Transition;
        NSFExternalTransition state3ToState2Transition;
        NSFExternalTransition state2ToState1Transition;

        void state2ReactionToEvent1Actions(const NSFStateMachineContext& context);
        void test1ReactionToEvent2Actions(const NSFStateMachineContext& context);
    };
}

#endif // STATE_MACHINE_RESTART_TEST_H