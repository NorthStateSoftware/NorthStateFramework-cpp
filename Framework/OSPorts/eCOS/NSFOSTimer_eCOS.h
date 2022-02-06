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

#ifndef NSF_OS_TIMER_ECOS_H
#define NSF_OS_TIMER_ECOS_H

#include "NSFOSTimer.h"

#include <cyg/kernel/kapi.h>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system timer in the eCOS environment.
    /// </summary>
    class NSFOSTimer_eCOS : public NSFOSTimer
    {
    public:

        /// <summary>
        /// Creates an operating system timer in the eCOS environment.
        /// </summary>
        /// <param name="name">The name of the timer.</param>
        NSFOSTimer_eCOS(const NSFString& name);

        /// <summary>
        /// Destroys an operating system timer in the eCOS environment.
        /// </summary>
        ~NSFOSTimer_eCOS();

        NSFTime getCurrentTime();

        virtual void setNextTimeout(NSFTime timeout);

        virtual void waitForNextTimeout();

    private:

        NSFTime startTime;

        cyg_alarm alarm;
        cyg_handle_t alarmHandle;

        cyg_sem_t semaphore;

        cyg_handle_t timerHandle;
        cyg_handle_t timerClockHandle;
        cyg_resolution_t timerResolution;
        
        NSFTime now();

        static void handleAlarm(cyg_handle_t alarm, cyg_addrword_t data);
    };
}

#endif // NSF_OS_TIMER_ECOS_H

