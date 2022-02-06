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

#ifndef STRATEGY_TEST_H
#define STRATEGY_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    class State2Strategy : public NSFStateMachine
    {
    public:

        State2Strategy(const NSFString& name, NSFCompositeState* parentState);
        ~State2Strategy();

        NSFEvent& getEvent3() { return event3; }
        NSFState& getState4() { return state4; }
        NSFState& getState5() { return state5; }

    private:

        // Events
        NSFEvent event3;

        // Regions and states, from outer to inner
        NSFInitialState intialState2;
        NSFCompositeState state4;
        NSFCompositeState state5;

        // Transitions, ordered internal, local, external
        NSFExternalTransition intialState2ToState4Transition;
        NSFExternalTransition state4ToState5Transition;
    };

    /// <summary>
    /// Test using state machine as substates.
    /// </summary>
    class StrategyTest : public NSFStateMachine, public ITestInterface
    {
    public:
        StrategyTest(const NSFString& name);

        ~StrategyTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

        void state5EntryActions(const NSFStateMachineContext& context);

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
        NSFExternalTransition initialToState1Transition;
        NSFExternalTransition state1ToState2Transition;
        NSFExternalTransition state2ToState3Transition;
        NSFExternalTransition state3ToState2Transition;
        NSFExternalTransition state2ToState1Transition;

        State2Strategy state2Strategy;
    };
}

#endif // STRATEGY_TEST_H