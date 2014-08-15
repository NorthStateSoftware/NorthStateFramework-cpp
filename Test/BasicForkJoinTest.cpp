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

#include "BasicForkJoinTest.h"

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    BasicForkJoinTest::BasicForkJoinTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),
        // Events
        event1("Event1", this),
        event2("Event2", this),

        //States
        // Region A
        regionA("RegionA", this),
        initialAState("InitialA", &regionA),
        stateA1("StateA1", &regionA, NULL, NULL),
        stateA2("StateA2", &regionA, NULL, NULL),

        // Region B
        regionB("RegionB", this),
        initialBState("InitialB", &regionB),
        stateB1("StateB1", &regionB, NULL, NULL),
        stateB2("StateB2", &regionB, NULL, NULL),

        // ForkJoins
        abForkJoin("ABForkJoin", this),

        // Transitions, ordered internal, local, external
        // Region A
        initialAStateToStateA1Transition("InitialAStateToStateA1", &initialAState, &stateA1, NULL, NULL, NULL),
        stateA1ToABForkJoinTransition("StateA1ToABForkJoin", &stateA1, &abForkJoin, &event1, NULL, NULL),
        abForkJoinToStateA2Transition("AbForkJoinToStateA2", &abForkJoin, &stateA2, NULL, NULL, NULL),

        // Region B
        initialBStateToStateB1Transition("InitialBStateToStateB1", &initialBState, &stateB1, NULL, NULL, NULL),
        stateB1ToABForkJoinTransition("StateB1ToABForkJoin", &stateB1, &abForkJoin, &event2, NULL, NULL),
        abForkJoinToStateB2Transition("AbForkJoinToStateB2", &abForkJoin, &stateB2, NULL, NULL, NULL)
    {
    }

    BasicForkJoinTest::~BasicForkJoinTest()
    {
        terminate(true);
        delete getEventThread();
    }

    bool BasicForkJoinTest::runTest(NSFString& errorMessage)
    {
        startStateMachine();

        // Test 
        //  state machine start up 
        //  initial state entering 
        //  null transition.
        //  composite states
        //  user regions
        //  congruent regions
        //  nested states
        //  entry actions
        if (!testHarness.doesEventResultInState(NULL, &stateA1))
        {
            errorMessage = "State Machine did not start properly with congruent states.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  Fork join entry
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &abForkJoin, &stateB1))
        {
            errorMessage = "State Machine did enter the fork join or did not stay in stateB1.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  Fork join entry evaluation
        //  Fork join exit
        //  entry actions
        if (!testHarness.doesEventResultInState(&event2, &stateA2, &stateB2))
        {
            errorMessage = "State Machine did exit the fork join properly.";
            stopStateMachine(); 
            return false;
        }


        stopStateMachine();
        return true;
    }

}
