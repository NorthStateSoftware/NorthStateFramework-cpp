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

#if defined NSF_OS_POSIX

#include "NSFOSSignal_POSIX.h"

#include <stdexcept>
#include <time.h>
#include <errno.h>
#include "NSFCoreTypes.h"
#include <cstring>

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSSignal* NSFOSSignal::create(const NSFString& name)
    {
        return new NSFOSSignal_POSIX(name);
    }

    // Concrete class definitions

    // Public

    NSFOSSignal_POSIX::NSFOSSignal_POSIX(const NSFString& name)
        : NSFOSSignal(name), signalSemaphore()
    {
        // Create signalSemaphore that is shared within process (first 0 arg), and with zero initial value (second 0 arg)
        if (sem_init(&signalSemaphore, 0, 0) != 0)
        {
            throw std::runtime_error(getName() + " signal sem_init() failed in constructor: " + toString(strerror(errno)));
        }
    }

    NSFOSSignal_POSIX::~NSFOSSignal_POSIX()
    {
        send();
        if (sem_destroy(&signalSemaphore) != 0)
        {
            throw std::runtime_error(getName() + " signal sem_destroy() failed in destructor: " + toString(strerror(errno)));
        }
    }

    void NSFOSSignal_POSIX::clear()
    {
        while (sem_trywait(&signalSemaphore) == 0);
    }

    void NSFOSSignal_POSIX::send()
    {
        if (sem_post(&signalSemaphore) != 0)
        {
            throw std::runtime_error(getName() + " signal sem_post() failed in send(): " + toString(strerror(errno)));
        }
    }

    bool NSFOSSignal_POSIX::wait()
    {
        while (sem_wait(&signalSemaphore) != 0)
        {
            switch (errno)
            {
            case EINTR:
                // The call was interrupted by a signal handler ... return to waiting for the signal.
                break;
            default:
                throw std::runtime_error(getName() + " signal sem_wait() failed in wait(): " + toString(strerror(errno)));
            }
        }
        return true;
    }

    bool NSFOSSignal_POSIX::wait(Int32 timeout)
    {
        // Compute absolute expiration time
        timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        timespec relativeTime = {timeout / MilliSecondsPerSecond, (timeout % MilliSecondsPerSecond) * NanoSecondsPerMilliSecond};
        timespec absTimeout = {now.tv_sec + relativeTime.tv_sec, now.tv_nsec + relativeTime.tv_nsec};
        if (absTimeout.tv_nsec >= NanoSecondsPerSecond)
        {
            ++absTimeout.tv_sec;
            absTimeout.tv_nsec -= NanoSecondsPerSecond;
        }

        while (sem_timedwait(&signalSemaphore, &absTimeout) != 0)
        {
            switch (errno)
            {
            case EINTR:
                // The call was interrupted by a signal handler ... return to waiting for the signal.
                break;
            case ETIMEDOUT:
                // The call timed out before the semaphore could be locked
                return false;
            default:
                throw std::runtime_error(getName() + " signal sem_timedwait() failed in wait(Int32 timeout): " + toString(strerror(errno)));
            }
        }

        return true;
    }
}

#endif // NSF_OS_POSIX
