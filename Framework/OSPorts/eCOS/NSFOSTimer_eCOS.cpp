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

#if defined NSF_OS_ECOS

#include "NSFOSTimer_eCOS.h"

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSTimer* NSFOSTimer::create(const NSFString& name)
    {
        return new NSFOSTimer_eCOS(name);
    }    

    // Concrete class definitions

    // Public 

    NSFOSTimer_eCOS::NSFOSTimer_eCOS(const NSFString& name)
        : NSFOSTimer(name)
    {
        // Using core real-time clock
        timerHandle = cyg_real_time_clock();
        timerResolution = cyg_clock_get_resolution(timerHandle);

        // Create semaphore to wake up timer on alarms
        cyg_semaphore_init(&semaphore, 0);

        // Create alarm to use as a timing mechanism
        cyg_clock_to_counter(timerHandle, &timerClockHandle);
        cyg_alarm_create(timerClockHandle, handleAlarm, (cyg_addrword_t) this, &alarmHandle, &alarm);

        startTime = now();
    }

    NSFOSTimer_eCOS::~NSFOSTimer_eCOS()
    {
        cyg_alarm_delete(alarmHandle);

        cyg_semaphore_post(&semaphore);
        cyg_semaphore_destroy(&semaphore);
    }

    NSFTime NSFOSTimer_eCOS::getCurrentTime()
    {
        return (now() - startTime);
    }

    void NSFOSTimer_eCOS::setNextTimeout(NSFTime timeout)
    {
        // Set alarm to expire at timeout (converted to eCOS absolute time)
        // Alarm will send semaphore to wait function upon expiration
        // Compute the time in ticks.  Assumes timerResolution.divisor is in Hz (ticks/sec), as per documentation.
        // ticks = (ms * ticks/sec) / 1000mS/1sec
        cyg_alarm_initialize(alarmHandle, ((startTime + timeout) * timerResolution.divisor) / MilliSecondsPerSecond, 0);
    }

    void NSFOSTimer_eCOS::waitForNextTimeout()
    {
        cyg_semaphore_wait(&semaphore);
    }

    // Private

    NSFTime NSFOSTimer_eCOS::now()
    {
        // Compute the time in mS.  Assumes timerResolution.divisor is in Hz (ticks/sec), as per documentation.
        // ms = (ticks * 1000mS/1sec) / (ticks/sec)
        return ((cyg_current_time() * MilliSecondsPerSecond) / timerResolution.divisor);
    }

    void NSFOSTimer_eCOS::handleAlarm(cyg_handle_t alarm, cyg_addrword_t data)
    {
        NSFOSTimer_eCOS* timer = (NSFOSTimer_eCOS*) data;
        cyg_semaphore_post(&(timer->semaphore));
    }
}

#endif // NSF_OS_ECOS   
