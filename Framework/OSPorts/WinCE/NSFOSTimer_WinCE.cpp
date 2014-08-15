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

