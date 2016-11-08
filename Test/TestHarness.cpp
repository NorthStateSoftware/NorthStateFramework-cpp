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

#include "TestHarness.h"

namespace NSFTest
{

    TestHarness::TestHarness()
    {
        firstSignal = NSFOSSignal::create("FirstSignal");
        secondSignal = NSFOSSignal::create("SecondSignal");
    }

    TestHarness::~TestHarness()
    {
        delete firstSignal;
        delete secondSignal;
    }

    bool TestHarness::doesEventResultInState(NSFEvent* eventToInject, NSFState* stateToWaitFor)
    {
        return doesEventResultInState(eventToInject, stateToWaitFor, DefaultWaitTime);
    }

    bool TestHarness::doesEventResultInState(NSFEvent* eventToInject, NSFState* stateToWaitFor, int waitTime)
    {
        firstSignal->clear();

        stateToWaitFor->EntryActions += NSFAction<NSFOSSignal, NSFStateMachineContext>(firstSignal, &NSFOSSignal::send);

        if (eventToInject != NULL)
        {
            eventToInject->queueEvent();
        }

        if (stateToWaitFor->isActive())
        {
            stateToWaitFor->EntryActions -= NSFAction<NSFOSSignal, NSFStateMachineContext>(firstSignal, &NSFOSSignal::send);
            return true;
        }

        if (!firstSignal->wait(waitTime))
        {
            stateToWaitFor->EntryActions -= NSFAction<NSFOSSignal, NSFStateMachineContext>(firstSignal, &NSFOSSignal::send);
            return false;
        }

        stateToWaitFor->EntryActions -= NSFAction<NSFOSSignal, NSFStateMachineContext>(firstSignal, &NSFOSSignal::send);
        return true;
    }

    bool TestHarness::doesEventResultInState(NSFEvent* eventToInject, NSFState* stateToWaitFor, NSFState* secondStateToWaitFor)
    {
        return doesEventResultInState(eventToInject, stateToWaitFor, secondStateToWaitFor, DefaultWaitTime);
    }

    bool TestHarness::doesEventResultInState(NSFEvent* eventToInject, NSFState* stateToWaitFor, NSFState* secondStateToWaitFor, int waitTime)
    {
        secondSignal->clear();

        secondStateToWaitFor->EntryActions += NSFAction<NSFOSSignal, NSFStateMachineContext>(secondSignal, &NSFOSSignal::send);

        if (!doesEventResultInState (eventToInject, stateToWaitFor, waitTime))
        {
            secondStateToWaitFor->EntryActions -= NSFAction<NSFOSSignal, NSFStateMachineContext>(secondSignal, &NSFOSSignal::send);
            return false;
        }

        if (secondStateToWaitFor->isActive())
        {
            secondStateToWaitFor->EntryActions -= NSFAction<NSFOSSignal, NSFStateMachineContext>(secondSignal, &NSFOSSignal::send);
            return true;
        }

        if (!secondSignal->wait(waitTime))
        {
            secondStateToWaitFor->EntryActions -= NSFAction<NSFOSSignal, NSFStateMachineContext>(secondSignal, &NSFOSSignal::send);
            return false;
        }

        secondStateToWaitFor->EntryActions -= NSFAction<NSFOSSignal, NSFStateMachineContext>(secondSignal, &NSFOSSignal::send);
        return true;
    }
}
