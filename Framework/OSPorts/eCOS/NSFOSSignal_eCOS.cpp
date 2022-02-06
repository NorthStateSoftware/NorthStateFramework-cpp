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
