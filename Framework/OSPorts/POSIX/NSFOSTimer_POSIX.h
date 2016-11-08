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

#ifndef NSF_OS_TIMER_POSIX_H
#define NSF_OS_TIMER_POSIX_H

#include "NSFOSTimer.h"

#include "NSFOSSignal.h"
#include <time.h>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system timer in the POSIX environment.
    /// </summary>
    class NSFOSTimer_POSIX : public NSFOSTimer
    {
    public:

        /// <summary>
        /// Creates an operating system timer in the POSIX environment.
        /// </summary>
        /// <param name="name">The name of the timer.</param>
        /// <param name="signalNumber">The signal number used by the timer.</param>
        NSFOSTimer_POSIX(const NSFString& name);

        /// <summary>
        /// Destroys an operating system timer in the POSIX environment.
        /// </summary>
        ~NSFOSTimer_POSIX();

        NSFTime getCurrentTime();

        virtual void setNextTimeout(NSFTime timeout);

        virtual void waitForNextTimeout();

    private:

        NSFTime nextTimeout;
        NSFTime startTime;
        NSFOSMutex* timerMutex;
        NSFOSSignal* timerSignal;

        static const clockid_t ClockNumber = CLOCK_REALTIME;

        NSFTime now();
    };
}

#endif // NSF_OS_TIMER_POSIX_H

