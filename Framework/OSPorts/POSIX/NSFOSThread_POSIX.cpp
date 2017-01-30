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

#if defined NSF_OS_POSIX

#include "NSFOSThread_POSIX.h"
#include <errno.h>
#include "NSFCoreTypes.h"
#include <cstring>


namespace NorthStateFramework
{
    // Base class definitions

    int NSFOSThread::getHighestPriority()
    {
        // POSIX scheduling priorities have larger numbers for higher priority
        return sched_get_priority_max(SCHED_FIFO) - 1;
    }

    int NSFOSThread::getHighPriority()
    {
        return (getMediumPriority() + getHighestPriority()) / 2;
    }

    int NSFOSThread::getMediumPriority()
    {
        return (getHighestPriority() + getLowestPriority()) / 2;
    }

    int NSFOSThread::getLowPriority()
    {
        return (getMediumPriority() + getLowestPriority()) / 2;
    }

    int NSFOSThread::getLowestPriority()
    {
        return sched_get_priority_min(SCHED_FIFO);
    }

    NSFOSThread* NSFOSThread::create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority)
    {
        return new NSFOSThread_POSIX(name, executionAction, priority);
    }

    NSFOSThread* NSFOSThread::create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
    {
        return new NSFOSThread_POSIX(name, executionAction);
    }

    void NSFOSThread::sleep(Int32 sleepTime)
    {
        if (sleepTime < 0)
        {
            sleepTime = 0;
        }

        timespec sleepTimespec = {sleepTime / MilliSecondsPerSecond, (sleepTime % MilliSecondsPerSecond) * NanoSecondsPerMilliSecond};
        NSFOSThread_POSIX::sleepUntil(&sleepTimespec);
    }

    // Concrete class definitions

    // Public

    NSFOSThread_POSIX::NSFOSThread_POSIX(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
        : NSFOSThread(name, executionAction), startSemaphore(), threadAttributes(), threadScheduleParameter(), thread()
    {
        construct(getMediumPriority());
    }

    NSFOSThread_POSIX::NSFOSThread_POSIX(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority)
        : NSFOSThread(name, executionAction), startSemaphore(), threadAttributes(), threadScheduleParameter(), thread()
    {
        construct(priority);
    }

    NSFOSThread_POSIX::~NSFOSThread_POSIX()
    {
        sem_destroy(&startSemaphore);
        pthread_attr_destroy(&threadAttributes);
    }

    int NSFOSThread_POSIX::getPriority() const
    {
        return threadScheduleParameter.sched_priority;
    }

    void NSFOSThread_POSIX::setPriority(int priority)
    {
        threadScheduleParameter.sched_priority = priority;

        if (pthread_setschedparam(thread, SCHED_FIFO, &threadScheduleParameter) != 0)
        {
            throw std::runtime_error(getName() + " thread pthread_setschedparam() failed in setPriority(int priority): " + toString(strerror(errno)));
        }
    }

    int NSFOSThread_POSIX::getOSThreadId()
    {
        // Possibly non-portable
        return thread;
    }

    void NSFOSThread_POSIX::startThread()
    {
        if (sem_post(&startSemaphore) != 0)
        {
            throw std::runtime_error(getName() + " thread sem_post() failed in startThread(): " + toString(strerror(errno)));
        }
    }

    void NSFOSThread_POSIX::sleepUntil(timespec* timeToSleep)
    {
        timespec remainder;
        while (nanosleep(timeToSleep, &remainder) != 0)
        {
            switch (errno)
            {
            case EINTR:
                (*timeToSleep) = remainder;
                break;
            default:
                throw std::runtime_error("Thread nanosleep() failed in sleepUntil(timespec* timeToSleep): " + toString(strerror(errno)));
            }
        }
    }
    // Private

    void NSFOSThread_POSIX::construct(int priority)
    {
        // Create startSemaphore that is shared within process (first 0 arg), and with zero initial value (second 0 arg)
        if (sem_init(&startSemaphore, 0, 0) != 0)
        {
            throw std::runtime_error(getName() + " thread sem_init() failed in constructor: " + toString(strerror(errno)));
        }

        // Set up attributes

        // Atttribute method signatures:
        // int pthread_attr_init(pthread_attr_t *attr);
        // int pthread_attr_destroy(pthread_attr_t *attr);
        // int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
        // int pthread_attr_getstack(const pthread_attr_t * restrict attr, void ** restrict stackaddr, size_t * restrict stacksize);
        // int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
        // int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
        // int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
        // int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);
        // int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
        // int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);
        // int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
        // int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
        // int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched);
        // int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched);
        // int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
        // int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);
        // int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
        // int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
        // int pthread_attr_setscope(pthread_attr_t *attr, int contentionscope);
        // int pthread_attr_getscope(const pthread_attr_t *attr, int *contentionscope);

        // Attribute default settings
        if (pthread_attr_init(&threadAttributes) != 0)
        {
            throw std::runtime_error(getName() + " thread pthread_attr_init() failed in constructor: " + toString(strerror(errno)));
        }

        // NSF attribute settings:

        // Detached - so that thread cleans up on return, without need for join
        if (pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_DETACHED) != 0)
        {
            throw std::runtime_error(getName() + " thread pthread_attr_setdetachstate() failed in constructor: " + toString(strerror(errno)));
        }

        // Stack address and size - so these are explicitely controlled
        if (pthread_attr_setstack(&threadAttributes, stack, StackSize) != 0)
        {
            throw std::runtime_error(getName() + " thread pthread_attr_setstack() failed in constructor: " + toString(strerror(errno)));
        }

        // Inherited scheduling off - so explicitely scheduling can be specified
//        if (pthread_attr_setinheritsched(&threadAttributes, PTHREAD_EXPLICIT_SCHED) != 0)
//        {
//            throw std::runtime_error(getName() + " thread attribute setinheritsched failed in constructor");
//        }

        // Schedule policy - so FIFO is guaranteed
        if (pthread_attr_setschedpolicy(&threadAttributes, SCHED_FIFO) != 0)
        {
            throw std::runtime_error(getName() + " thread pthread_attr_setschedpolicy() failed in constructor: " + toString(strerror(errno)));
        }

        // Schedule priority - so proper priority is set
        threadScheduleParameter.sched_priority = priority;
        if (pthread_attr_setschedparam(&threadAttributes, &threadScheduleParameter) != 0)
        {
            throw std::runtime_error(getName() + " thread pthread_attr_setschedparam() failed in constructor: " + toString(strerror(errno)));
        }

        // Scope - so system wide scheduling scope is set
        if (pthread_attr_setscope(&threadAttributes, PTHREAD_SCOPE_SYSTEM) != 0)
        {
            throw std::runtime_error(getName() + " thread pthread_attr_setscope() failed in constructor: " + toString(strerror(errno)));
        }

        // Create thread
        if (pthread_create(&thread, &threadAttributes, threadEntry, this) != 0)
        {
            throw std::runtime_error(getName() + " thread pthread_create() failed in constructor: " + toString(strerror(errno)));
        }
    }

    void* NSFOSThread_POSIX::threadEntry(void* thread)
    {
        NSFOSThread_POSIX* posixThread = (NSFOSThread_POSIX*)thread;

        // Wait for thread to be started via startThread()
        while (sem_wait(&posixThread->startSemaphore) != 0)
        {
            switch (errno)
            {
            case EINTR:
                // The call was interrupted by a signal handler ... return to waiting for the signal.
                break;
            default:
                throw std::runtime_error(posixThread->getName() + " thread sem_wait() failed in threadEntry(void* thread): " + toString(strerror(errno)));
            }
        }

        posixThread->executeAction();

        return NULL;
    }
}

#endif // NSF_OS_POSIX

