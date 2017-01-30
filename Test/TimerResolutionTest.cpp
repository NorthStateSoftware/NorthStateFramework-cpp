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

#include "TimerResolutionTest.h"
#include <limits.h>


namespace NSFTest
{
    TimerResolutionTest::TimerResolutionTest(const NSFString& name)
        : name(name.c_str())
    {
    }

    bool TimerResolutionTest::runTest(NSFString&)
    {
        NSFTime startTime = NSFTimerThread::getPrimaryTimerThread().getCurrentTime();
        NSFTime lastTime = startTime;
        NSFTime maxDeltaTime = 0;
        NSFTime minDeltaTime = LONG_MAX;

        while(true)
        {
            NSFTime currentTime = NSFTimerThread::getPrimaryTimerThread().getCurrentTime();

            if (currentTime != lastTime)
            {
                NSFTime deltaTime = currentTime - lastTime;

                // Record max and min observed delta times
                if (deltaTime > maxDeltaTime) { maxDeltaTime = deltaTime; }
                if (deltaTime < minDeltaTime) { minDeltaTime = deltaTime; }
            }

            // Record current time as last time through loop
            lastTime = currentTime;

            // End after 1000 mS
            if (currentTime > startTime + 1000)
            {
                break;
            }
        }

        // Add results to name for test visibility
        name += "; Min / Max Delta Time = " + toString(minDeltaTime) + " / " + toString(maxDeltaTime) + " mS";

        return true;
    }
}
