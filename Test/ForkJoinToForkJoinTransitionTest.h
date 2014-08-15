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

#ifndef FORK_JOIN_TO_FORK_JOIN_TRANSITION_TEST_H
#define FORK_JOIN_TO_FORK_JOIN_TRANSITION_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test Fork Join to Fork Join Transitions
    /// Extra regional transitions to ForkJoins
    /// </summary>
    class ForkJoinToForkJoinTransitionTest : public NSFStateMachine, public ITestInterface
    {
    public:
        ForkJoinToForkJoinTransitionTest(const NSFString& name);

        ~ForkJoinToForkJoinTransitionTest();

        const NSFString& getName() { return NSFStateMachine::getName();}

        bool runTest(NSFString& errorMessage);

    private:

        TestHarness testHarness;

        // Fork Joins
        NSFForkJoin bSynch;
        NSFForkJoin abForkJoin;
        NSFForkJoin bcForkJoin;

        // ARegion
        // Events
        NSFEvent evA1;

        // Regions and states, from outer to inner
        NSFRegion aRegion;
        NSFInitialState initialAState;
        NSFCompositeState stateA1;
        NSFCompositeState stateA2;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialAToStateA1Transition;
        NSFExternalTransition stateA1ToABForkJoinTransition;
        NSFExternalTransition abForkJoinToA2Transition;

        // B Region
        // Events
        NSFEvent evB1;

        // Regions and states, from outer to inner
        NSFRegion bRegion;
        NSFInitialState initialBState;
        NSFCompositeState stateB1;
        NSFCompositeState stateB2;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialBToStateB1Transition;
        NSFExternalTransition stateB1ToBSynchTransition;
        NSFExternalTransition bSynchToStateB2Transition;

        // C Region
        // Events
        NSFEvent evC1;

        // Regions and states, from outer to inner
        NSFRegion cRegion;
        NSFInitialState initialCState;
        NSFCompositeState stateC1;
        NSFCompositeState stateC2;

        // Transitions, ordered internal, local, external
        NSFExternalTransition initialCToStateC1Transition;
        NSFExternalTransition stateC1ToBCForkJoinTransition;
        NSFExternalTransition bcForkJoinToC2Transition;

        // Extra Regional Transitions
        NSFExternalTransition bSynchToABForkJoinTransition;
        NSFExternalTransition bSynchToBCForkJoinTransition;
    };
}

#endif // FORK_JOIN_TO_FORK_JOIN_TRANSITION_TEST_H