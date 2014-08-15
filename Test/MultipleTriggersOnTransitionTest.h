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


#ifndef MULTIPLE_TRIGGERS_ON_TRANSITION_TEST_H
#define MULTIPLE_TRIGGERS_ON_TRANSITION_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test multiple triggers for a single transition.
    /// </summary>
    class MultipleTriggersOnTransitionTest : public NSFStateMachine, public ITestInterface
    {
    public:
        MultipleTriggersOnTransitionTest(const NSFString& name);

        ~MultipleTriggersOnTransitionTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private:

        TestHarness testHarness;

        // Events
        NSFEvent event1;
        NSFEvent event2;
        NSFEvent event3;
        NSFEvent event4;

        // Regions and states, from outer to inner
        NSFInitialState initialState;
        NSFCompositeState state1;
        NSFCompositeState state2;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialAToState1Transition;
        NSFExternalTransition state1ToState2Transition;
        NSFExternalTransition state2ToState1Transition;
    };
}

#endif // MULTIPLE_TRIGGERS_ON_TRANSITION_TEST_H