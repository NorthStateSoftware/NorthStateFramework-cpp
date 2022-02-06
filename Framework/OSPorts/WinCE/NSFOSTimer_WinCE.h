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

#ifndef NSF_OS_TIMER_WINCE_H
#define NSF_OS_TIMER_WINCE_H

#include "NSFOSTimer.h"

#include "NSFOSSignal.h"
#include <windows.h>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system timer in the WinCE environment.
    /// </summary>
    class NSFOSTimer_WinCE : public NSFOSTimer
    {
    public:

        /// <summary>
        /// Creates an operating system timer in the WinCE environment.
        /// </summary>
        /// <param name="name">The name of the timer.</param>
        NSFOSTimer_WinCE(const NSFString& name);

        /// <summary>
        /// Destroys an operating system timer in the WinCE environment.
        /// </summary>
        ~NSFOSTimer_WinCE();

        NSFTime getCurrentTime();

        virtual void setNextTimeout(NSFTime timeout);

        virtual void waitForNextTimeout();

    private:

        NSFTime nextTimeout;
        NSFTime startTime;
        NSFOSMutex* timerMutex;
        NSFOSSignal* timerSignal;

        static const int OneHundredNanoSecondsPerMilliSecond = 10000;

        NSFTime now();
    };
}

#endif // NSF_OS_TIMER_WINCE_H

