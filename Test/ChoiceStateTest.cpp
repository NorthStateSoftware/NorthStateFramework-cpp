// Copyright 2004-2016, North State Software, LLC.  All rights reserved.

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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