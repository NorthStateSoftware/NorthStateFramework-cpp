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

#include "ExceptionHandlingTest.h"

using namespace NorthStateFramework;

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    ExceptionHandlingTest::ExceptionHandlingTest(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),
        // Events
        event1("Event1", this),
        event2("Event2", this),

        // Regions and states, from outer to inner
        initialState("InitialExceptionHandlingTest", this),
        state1("State1", this, NULL, NULL),
        state2("State2", this, NSFAction(this, &ExceptionHandlingTest::state2EntryActions), NULL),
        state3("State3", this, NULL, NULL),
        //Transitions
        initialToState1Transition("InitialToState1", &initialState, &state1, NULL, NULL, NULL),
        state1ToState2Transition("State1ToState2", &state1, &state2, &event1, NULL, NULL),
        state2ToState3Transition("State2ToState3", &state2, &state3, &event2, NULL, NULL),
        state3ToState2Transition("State3ToState2", &state3, &state2, &event1, NULL, NULL),
        state2ToState1Transition("State2ToState1", &state2, &state1, &event1, NULL, NULL)
    {
        ExceptionActions += NSFAction(this, &ExceptionHandlingTest::localHandleException);
    }

    ExceptionHandlingTest::~ExceptionHandlingTest()
    {
        terminate(true);
        delete getEventThread();
    }

    bool ExceptionHandlingTest::runTest(NSFString& errorMessage)
    {
        startStateMachine();

        // Test 
        //  state machine start up 
        //  initial state entering 
        //  null transition.
        //  entry actions
        if (!testHarness.doesEventResultInState(NULL, &state1))
        {
            errorMessage = "State Machine did not start properly.";
            stopStateMachine(); 
            return false;
        }

        // Test 
        //  state machine event handling
        //  triggered transitions
        //  exception handling
        //  entry actions
        if (!testHarness.doesEventResultInState(&event1, &state3))
        {
            errorMessage = "State Machine did not handle exception properly.";
            stopStateMachine(); 
            return false;
        }

        stopStateMachine();
        return true;
    }

    void ExceptionHandlingTest::state2EntryActions(const NSFStateMachineContext&)
    {
        throw std::runtime_error(IntentionalExceptionString());
    }

    void ExceptionHandlingTest::localHandleException(const NSFExceptionContext&)
    {
        event2.queueEvent();
    }
}