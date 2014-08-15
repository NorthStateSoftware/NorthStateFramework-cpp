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

#if defined NSF_OS_ECOS

#include "NSFOSSignal_eCOS.h"

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSSignal* NSFOSSignal::create(const NSFString& name)
    {
        return new NSFOSSignal_eCOS(name);
    }

    // Concrete class definitions

    // Public

    NSFOSSignal_eCOS::NSFOSSignal_eCOS(const NSFString& name)
        : NSFOSSignal(name)
    {
        cyg_flag_init(&flag);
    }

    NSFOSSignal_eCOS::~NSFOSSignal_eCOS()
    {
        cyg_flag_destroy(&flag);
    }

    void NSFOSSignal_eCOS::clear()
    {
        cyg_flag_maskbits(&flag, 0);
    }

    void NSFOSSignal_eCOS::send()
    {
        cyg_flag_setbits(&flag, 1);
    }

    bool NSFOSSignal_eCOS::wait()
    {
        return cyg_flag_wait(&flag, 1, CYG_FLAG_WAITMODE_AND | CYG_FLAG_WAITMODE_CLR);
    }

    bool NSFOSSignal_eCOS::wait(Int32 timeout)
    {
        static cyg_resolution_t timerResolution = cyg_clock_get_resolution(cyg_real_time_clock());

        // Set flag wait to expire at timeout (converted to eCOS absolute time)
        // Compute the time in ticks.  Assumes timerResolution.divisor is in Hz (ticks/sec), as per documentation.
        // ticks = current ticks + (ms * ticks/sec) / 1000mS/1sec
        return cyg_flag_timed_wait(&flag, 1, CYG_FLAG_WAITMODE_AND | CYG_FLAG_WAITMODE_CLR, cyg_current_time() + ((NSFTime)timeout * timerResolution.divisor) / MilliSecondsPerSecond);
    }
}

#endif // NSF_OS_ECOS
