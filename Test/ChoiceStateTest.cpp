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

#include "ChoiceStateTest.h"

using namespace NorthStateFramework;

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    ChoiceStateTest::ChoiceStateTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)), value (0),
        // Events
        evaluateEvent("EvaluateEvent", this),
        waitEvent("WaitEvent", this), 

        // Regions and states, from outer to inner
        initialChoiceStateTestState("InitialChoiceStateTest", this),
        waitToEvaluateState("WaitToEvaluate", this, NULL, NULL),
        evaluateValueState("EvaluateValue", this),
        evaluatedState("Evaluated", this, NULL, NULL),
        initialEvaluatedState("InitialEvaluatedState", &evaluatedState),
        valueLowState("ValueLow", &evaluatedState, NULL, NULL),
        valueMiddleState("ValueMiddle", &evaluatedState, NULL, NULL),
        valueHighState("ValueHigh", &evaluatedState, NULL, NULL),

        // Transitions, ordered internal, local, external
        initialChoiceStateTestToWaitToEvaluateTransition("InitialChoiceStateTestToWaitToEvaluate", &initialChoiceStateTestState, &waitToEvaluateState, NULL, NULL, NULL),
        waitToEvaluateToEvaluateValueTransition("WaitToEvaluateToEvaluateValueTransition", &waitToEvaluateState, &evaluateValueState, &evaluateEvent, NULL, NULL),
        initialEvaluatedToValueLowTransition("InitialEvaluatedToValueLowTransition", &initialEvaluatedState, &valueLowState, NULL, NULL, NULL),
        evaluateValueToValueLowTransition("EvaluateValueToValueLowTransition", &evaluateValueState, &valueLowState, NULL, NSFGuard(this, &ChoiceStateTest::isValueLow), NULL),
        evaluateValueToValueMiddleTransition("EvaluateValueToValueMiddleTransition", &evaluateValueState, &valueMiddleState, NULL, Else, NULL),
        evaluateValueToValueHighTransition("EvaluateValueToValueHighTransition", &evaluateValueState, &valueHighState, NULL, NSFGuard(this, &ChoiceStateTest::isValueHigh), NULL),
        evaluatedToWaitToEvaluateTransition("EvaluatedToWaitToEvaluateTransition", &evaluatedState, &waitToEvaluateState, &waitEvent, NULL, NSFAction(this, &ChoiceStateTest::addValue))
    {
    }

    ChoiceStateTest::~ChoiceStateTest()
    {
        terminate(true);
        delete getEventThread();
    }

    bool ChoiceStateTest::runTest(NSFString& errorMessage)
    {
        startStateMachine();

        if (!testHarness.doesEventResultInState(NULL, &waitToEvaluateState))
        {
            errorMessage = "State Machine did not start properly";
            stopStateMachine(); 
            return false;
        }

        if (!testHarness.doesEventResultInState(&evaluateEvent, &valueLowState))
        {
            errorMessage = "Choice State did not choose low";
            stopStateMachine(); 
            return false;
        }

        if (!testHarness.doesEventResultInState(&waitEvent, &waitToEvaluateState))
        {
            errorMessage = "Simple Transition failed from valueLowState to waitToEvaluateState";
            stopStateMachine(); 
            return false;
        }

        if (!testHarness.doesEventResultInState(&evaluateEvent, &valueMiddleState))
        {
            errorMessage = "Choice State did not choose middle";
            stopStateMachine(); 
            return false;
        }

        if (!testHarness.doesEventResultInState(&waitEvent, &waitToEvaluateState))
        {
            errorMessage = "Simple Transition failed from valueMiddleState to waitToEvaluateState";
            stopStateMachine(); 
            return false;
        }

        if (!testHarness.doesEventResultInState(&evaluateEvent, &valueHighState))
        {
            errorMessage = "Choice State did not choose high";
            stopStateMachine(); 
            return false;
        }

        stopStateMachine();
        return true;
    }

    bool ChoiceStateTest::isValueLow(const NSFStateMachineContext&)
    {
        return (value < 10);
    }

    bool ChoiceStateTest::isValueHigh(const NSFStateMachineContext&)
    {
        return (value > 10);
    }

    void ChoiceStateTest::addValue(const NSFStateMachineContext&)
    {
        value += 10;
    }
}