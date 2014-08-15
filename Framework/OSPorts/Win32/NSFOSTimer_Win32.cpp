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

#if defined NSF_OS_WIN32

#include "NSFOSTimer_Win32.h"

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSTimer* NSFOSTimer::create(const NSFString& name)
    {
        return new NSFOSTimer_Win32(name);
    }    

    // Concrete class definitions

    // Public 

    NSFOSTimer_Win32::NSFOSTimer_Win32(const NSFString& name)
        : NSFOSTimer(name), startTime(now())
    {
        if (!(timerHandle = CreateWaitableTimer(NULL, false, NULL)))
        {
            throw std::runtime_error(getName() + " unable to create waitable timer");
        }
    }

    NSFOSTimer_Win32::~NSFOSTimer_Win32()
    {
        if (timerHandle != NULL)
        {
            CloseHandle(timerHandle);
            timerHandle = NULL;
        }
    }

    NSFTime NSFOSTimer_Win32::getCurrentTime()
    {
        return (now() - startTime);
    }

    void NSFOSTimer_Win32::setNextTimeout(NSFTime timeout)
    {
        // Compute relative time for waitable timer call.
        // The timerDueTime is in 100 nanoSecond units, negative indicates relative time.
        timerDueTime.QuadPart = (getCurrentTime() - timeout) * OneHundredNanoSecondsPerMilliSecond;

        if (!SetWaitableTimer(timerHandle, &timerDueTime, 0, NULL, NULL, false))
        {
            throw std::runtime_error(getName() + " unable to set waitable timer");
        }
    }

    void NSFOSTimer_Win32::waitForNextTimeout()
    {
        if (WaitForSingleObject(timerHandle, INFINITE) != WAIT_OBJECT_0)
        {
            throw std::runtime_error(getName() + " waitable timer not signaled");
        }
    }

    // Private

    NSFTime NSFOSTimer_Win32::now()
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

#endif // NSF_OS_WIN32

