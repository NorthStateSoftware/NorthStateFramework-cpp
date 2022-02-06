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
        sem_post(&signalSemaphore);
        sem_destroy(&signalSemaphore);
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
