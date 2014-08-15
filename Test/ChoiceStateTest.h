// Copyright 2004-2014, North State Software, LLC.  All rights reserved.

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