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

#ifndef CHOICE_STATE_TEST_H
#define CHOICE_STATE_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test Basic State Machine Behavior
    /// </summary>
    class ChoiceStateTest : public NSFStateMachine, public ITestInterface
    {
    public:
        ChoiceStateTest(const NSFString& name);

        ~ChoiceStateTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private:

        int value;
        TestHarness testHarness;

        //Events
        NSFEvent evaluateEvent;
        NSFEvent waitEvent;

        // Regions and states, from outer to inner
        NSFInitialState initialChoiceStateTestState;
        NSFCompositeState waitToEvaluateState;
        NSFChoiceState evaluateValueState;
        NSFCompositeState evaluatedState;
        NSFInitialState initialEvaluatedState;
        NSFCompositeState valueLowState;
        NSFCompositeState valueMiddleState;
        NSFCompositeState valueHighState;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialChoiceStateTestToWaitToEvaluateTransition;
        NSFExternalTransition waitToEvaluateToEvaluateValueTransition;
        NSFExternalTransition initialEvaluatedToValueLowTransition;
        NSFExternalTransition evaluateValueToValueLowTransition;
        NSFExternalTransition evaluateValueToValueMiddleTransition;
        NSFExternalTransition evaluateValueToValueHighTransition;
        NSFExternalTransition evaluatedToWaitToEvaluateTransition;

    private:
        bool isValueLow(const NSFStateMachineContext& context);
        bool isValueHigh(const NSFStateMachineContext& context);
        void addValue(const NSFStateMachineContext& context);
    };
}

#endif // CHOICE_STATE_TEST_H