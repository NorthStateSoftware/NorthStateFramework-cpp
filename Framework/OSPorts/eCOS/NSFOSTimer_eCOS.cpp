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
