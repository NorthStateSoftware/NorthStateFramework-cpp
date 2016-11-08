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

