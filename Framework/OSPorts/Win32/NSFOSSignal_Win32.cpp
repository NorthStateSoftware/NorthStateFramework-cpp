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

#if defined NSF_OS_WIN32

#include "NSFOSSignal_Win32.h"

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSSignal* NSFOSSignal::create(const NSFString& name)
    {
        return new NSFOSSignal_Win32(name);
    }

    // Concrete class definitions

    // Public

    NSFOSSignal_Win32::NSFOSSignal_Win32(const NSFString& name)
        : NSFOSSignal(name), handle(NULL)
    {
        if ((handle = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
        {
            throw std::runtime_error(getName() + " unable to create signal");
        }
    }

    NSFOSSignal_Win32::~NSFOSSignal_Win32()
    {
        if (handle != NULL)
        {
            CloseHandle(handle);
        }
    }

    void NSFOSSignal_Win32::clear()
    {
        wait(0);
    }

    void NSFOSSignal_Win32::send()
    {
        SetEvent(handle);
    }

    bool NSFOSSignal_Win32::wait()
    {
        return wait(INFINITE);
    }

    bool NSFOSSignal_Win32::wait(Int32 timeout)
    {
        if (WaitForSingleObject(handle, timeout) != WAIT_OBJECT_0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

#endif // NSF_OS_WIN32

