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
