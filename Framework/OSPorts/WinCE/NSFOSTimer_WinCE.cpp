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

#if defined NSF_OS_WINCE

#include "NSFOSTimer_WinCE.h"


namespace NorthStateFramework
{
    // Base class definitions

    NSFOSTimer* NSFOSTimer::create(const NSFString& name)
    {
        return new NSFOSTimer_WinCE(name);
    }    

    // Concrete class definitions

    // Public 

    NSFOSTimer_WinCE::NSFOSTimer_WinCE(const NSFString& name)
        : NSFOSTimer(name), nextTimeout(INT_MAX), startTime(now()), timerMutex(NSFOSMutex::create()), timerSignal(NSFOSSignal::create(name))
    {
    }

    NSFOSTimer_WinCE::~NSFOSTimer_WinCE()
    {
        delete timerMutex;
        delete timerSignal;
    }

    NSFTime NSFOSTimer_WinCE::getCurrentTime()
    {
        return (now() - startTime);
    }

    void NSFOSTimer_WinCE::setNextTimeout(NSFTime timeout)
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

    void NSFOSTimer_WinCE::waitForNextTimeout()
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

    NSFTime NSFOSTimer_WinCE::now()
    {
        FILETIME fileNow;
        GetSystemTimeAsFileTime(&fileNow);

        ULARGE_INTEGER now;
        now.LowPart = fileNow.dwLowDateTime;
        now.HighPart = fileNow.dwHighDateTime;

        // File time is in 100 nanosecond intervals
        return (now.QuadPart / OneHundredNanoSecondsPerMilliSecond);
    }
}

#endif // NSF_OS_WINCE

