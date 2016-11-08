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

#include "NSFOSMutex_eCOS.h"
#include <stdexcept>

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSMutex* NSFOSMutex::create()
    {
        return new NSFOSMutex_eCOS();
    }

    // Concrete class definitions

    // Public

    NSFOSMutex_eCOS::NSFOSMutex_eCOS()
        : NSFOSMutex()
    {
        cyg_mutex_init(&mutex);
    }

    NSFOSMutex_eCOS::~NSFOSMutex_eCOS()
    {
        cyg_mutex_destroy(&mutex);
    }

    void NSFOSMutex_eCOS::lock()
    {
        if (cyg_mutex_lock(&mutex) == false)
        {
            throw std::runtime_error("Mutex failed to lock");
        }
    }

    void NSFOSMutex_eCOS::unlock()
    {
        cyg_mutex_unlock(&mutex);
    }
}

#endif // NSF_OS_ECOS

