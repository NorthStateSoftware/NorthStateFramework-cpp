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
        timerHandle = CreateWaitableTimer(NULL, false, NULL);

        if (timerHandle == NULL)
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

