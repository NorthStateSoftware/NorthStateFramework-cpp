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

#include "NSFCustomConfig.h"

#if defined NSF_OS_POSIX

#include "NSFOSTimer_POSIX.h"
#include "limits.h"

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSTimer* NSFOSTimer::create(const NSFString& name)
    {
        return new NSFOSTimer_POSIX(name);
    }

    // Concrete class definitions

    // Public

    NSFOSTimer_POSIX::NSFOSTimer_POSIX(const NSFString& name)
        : NSFOSTimer(name), nextTimeout(INT_MAX), startTime(now()), timerMutex(NSFOSMutex::create()), timerSignal(NSFOSSignal::create(name))
    {
    }

    NSFOSTimer_POSIX::~NSFOSTimer_POSIX()
    {
        delete timerMutex;
        delete timerSignal;
    }

    NSFTime NSFOSTimer_POSIX::getCurrentTime()
    {
        return (now() - startTime);
    }

    void NSFOSTimer_POSIX::setNextTimeout(NSFTime timeout)
    {
        bool sendSignal = false;
        LOCK(timerMutex)
        {
            // If new timeout is before next timeout, wake up timer blocked in waitForNextTimeout, so that it can set up the new shorter timeout
            if (timeout < nextTimeout)
            {
                sendSignal = true;
            }
            nextTimeout = timeout;
        }
        ENDLOCK;

        if (sendSignal)
        {
            timerSignal->send();
        }
    }

    void NSFOSTimer_POSIX::waitForNextTimeout()
    {
        NSFTime relativeTimeout;
        LOCK(timerMutex)
        {
            relativeTimeout = nextTimeout - getCurrentTime();
        }
        ENDLOCK;

        // Only wait if relative time indicates next timeout has not already passed
        if (relativeTimeout > 0)
        {
            Int32 waitTime;

            if (relativeTimeout > INT_MAX)
            {
                waitTime = INT_MAX;
            }
            else
            {
                waitTime = (Int32)relativeTimeout;
            }

            timerSignal->wait(waitTime);
        }
    }

    // Private

    NSFTime NSFOSTimer_POSIX::now()
    {
        timespec timespecNow;
        clock_gettime(ClockNumber, &timespecNow);

        NSFTime now = (timespecNow.tv_sec * MilliSecondsPerSecond) + (timespecNow.tv_nsec / NanoSecondsPerMilliSecond);

        return now;
    }
}

#endif // NSF_OS_POSIX
