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

#include "TimerAccuracyTest.h"
#include <limits.h>

namespace NSFTest
{
    TimerAccuracyTest::TimerAccuracyTest(const NSFString& name)
        : name(name.c_str()), eventHandler(name, new NSFEventThread(name)), testEvent(name, &eventHandler),
        lastTime(0), maxDeltaTime(0), minDeltaTime(LONG_MAX)
    {
        eventHandler.startEventHandler();
    }

    TimerAccuracyTest::~TimerAccuracyTest()
    {
        delete eventHandler.getEventThread();
    }

    bool TimerAccuracyTest::runTest(NSFString&)
    {
        eventHandler.addEventReaction(&testEvent, NSFAction(this, &TimerAccuracyTest::testEventAction));

        // Schedule test event to fire every millisecond
        testEvent.schedule(0, 1);

        // Wait for test to end
        NSFOSThread::sleep(1000);

        // Unschedule test event
        testEvent.unschedule();

        // Add results to name for test visibility
        name += "; Min / Max Delta Time = " + toString(minDeltaTime) + " / " + toString(maxDeltaTime) + " mS";

        return true;
    }

    void TimerAccuracyTest::testEventAction(const NSFEventContext&)
    {
        NSFTime currentTime = NSFTimerThread::getPrimaryTimerThread().getCurrentTime();

        if (lastTime == 0)
        {
            lastTime = currentTime;
            return;
        }

        NSFTime deltaTime = currentTime - lastTime;

        // Record max and min observed delta times
        if (deltaTime > maxDeltaTime) { maxDeltaTime = deltaTime; }
        if (deltaTime < minDeltaTime) { minDeltaTime = deltaTime; }

        // Record current time as last time through loop
        lastTime = currentTime;
    }
}
