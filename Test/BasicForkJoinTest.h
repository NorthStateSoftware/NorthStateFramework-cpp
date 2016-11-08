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