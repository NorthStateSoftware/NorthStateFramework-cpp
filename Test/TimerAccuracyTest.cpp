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

    bool TimerAccuracyTest::runTest(NSFString& errorMessage)
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

    void TimerAccuracyTest::testEventAction(const NSFEventContext& context)
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
