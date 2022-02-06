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
