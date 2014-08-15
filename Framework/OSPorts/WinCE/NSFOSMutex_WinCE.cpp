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

#if defined NSF_OS_WINCE

#include "NSFOSMutex_WinCE.h"

#include <stdexcept>

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSMutex* NSFOSMutex::create()
    {
        return new NSFOSMutex_WinCE();
    }

    // Concrete class definitions

    // Public

    NSFOSMutex_WinCE::NSFOSMutex_WinCE()
#if defined _DEBUG
        : lockCounter(0)
#endif  // _DEBUG
    {
        InitializeCriticalSection(&criticalSection);
    }

    NSFOSMutex_WinCE::~NSFOSMutex_WinCE()
    {
        DeleteCriticalSection(&criticalSection);
    }

    void NSFOSMutex_WinCE::lock()
    {
        EnterCriticalSection(&criticalSection);

#if defined _DEBUG
        if (++lockCounter > 1)
        {
            // Not all operating systems support recursive mutex locks, 
            // this performs a runtime check to ensure that those do not happen.
            throw std::runtime_error("Recursive Lock Detected");
        }
#endif  // _DEBUG
    }

    void NSFOSMutex_WinCE::unlock()
    {
#if defined _DEBUG
        --lockCounter;
#endif  // _DEBUG

        LeaveCriticalSection(&criticalSection);
    }
}

#endif // NSF_OS_WINCE
