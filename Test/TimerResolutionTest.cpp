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
