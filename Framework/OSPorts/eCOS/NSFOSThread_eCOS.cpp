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

#include "NSFOSThread_eCOS.h"

#define THREAD_PRIORITY_LOWEST (CYGNUM_KERNEL_SCHED_PRIORITIES / 2)
#define THREAD_PRIORITY_HIGHEST 1
#define THREAD_PRIORITY_NORMAL (THREAD_PRIORITY_LOWEST + THREAD_PRIORITY_HIGHEST) / 2
#define THREAD_PRIORITY_ABOVE_NORMAL (THREAD_PRIORITY_HIGHEST + THREAD_PRIORITY_NORMAL) / 2
#define THREAD_PRIORITY_BELOW_NORMAL (THREAD_PRIORITY_NORMAL + THREAD_PRIORITY_LOWEST) / 2

namespace NorthStateFramework
{
    // Base class definitions

    int NSFOSThread::getHighestPriority()
    {
        return THREAD_PRIORITY_HIGHEST;
    }

    int NSFOSThread::getHighPriority()
    {
        return THREAD_PRIORITY_ABOVE_NORMAL;
    }

    int NSFOSThread::getMediumPriority()
    {
        return THREAD_PRIORITY_NORMAL;
    }

    int NSFOSThread::getLowPriority()
    {
        return THREAD_PRIORITY_BELOW_NORMAL;
    }

    int NSFOSThread::getLowestPriority()
    {
        return THREAD_PRIORITY_LOWEST;
    }

    NSFOSThread* NSFOSThread::create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority)
    {
        return new NSFOSThread_eCOS(name, executionAction, priority);
    }    

    NSFOSThread* NSFOSThread::create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
    {
        return new NSFOSThread_eCOS(name, executionAction);
    }    

    void NSFOSThread::sleep(UInt32 sleepTime)
    {
        static cyg_resolution_t timerResolution = cyg_clock_get_resolution(cyg_real_time_clock());

        // Assumes cyg_clock_get_resolution(cyg_real_time_clock()).divisor is in Hz (ticks/sec), as per documentation.
        cyg_thread_delay(((sleepTime * timerResolution.divisor) + 999) / 1000);
    }

    // Concrete class definitions

    // Public

    NSFOSThread_eCOS::NSFOSThread_eCOS(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
        : NSFOSThread(name, executionAction) 
    {
        construct(getMediumPriority());
    }

    NSFOSThread_eCOS::NSFOSThread_eCOS(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority)
        : NSFOSThread(name, executionAction) 
    {
        construct(priority);
    }

    NSFOSThread_eCOS::~NSFOSThread_eCOS()
    {
        cyg_thread_delete(handle);
    }

    int NSFOSThread_eCOS::getPriority() const
    {
        return priority;
    }

    void NSFOSThread_eCOS::setPriority(int priority)
    {
        this->priority = priority;
        cyg_thread_set_priority(handle, priority);
    }

    int NSFOSThread_eCOS::getOSThreadId()
    {
        return handle;
    }

    void NSFOSThread_eCOS::startThread()
    {
        cyg_thread_resume(handle);
    }

    // Private

    void NSFOSThread_eCOS::construct(int priority)
    {
        cyg_thread_create(priority, &NSFOSThread_eCOS::threadEntry, (cyg_addrword_t)this, (char *)getName().c_str(), stack, StackSize, &handle, &thread);
    }

    void NSFOSThread_eCOS::threadEntry(cyg_addrword_t nsfOSThread)
    {
        ((NSFOSThread*)nsfOSThread)->executeAction();
    }
}

#endif // NSF_OS_ECOS

