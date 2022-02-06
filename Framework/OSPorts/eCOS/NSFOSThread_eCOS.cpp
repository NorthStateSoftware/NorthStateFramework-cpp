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

    void NSFOSThread::sleep(Int32 sleepTime)
    {
        static cyg_resolution_t timerResolution = cyg_clock_get_resolution(cyg_real_time_clock());

        if (sleepTime < 0)
        {
            sleepTime = 0;
        }

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

