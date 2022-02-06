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

#include "NSFOSMutex_POSIX.h"
#include <stdexcept>
#include <errno.h>
#include "NSFCoreTypes.h"
#include <cstring>

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSMutex* NSFOSMutex::create()
    {
        return new NSFOSMutex_POSIX();
    }

    // Concrete class definitions

    // Public

    NSFOSMutex_POSIX::NSFOSMutex_POSIX()
        : NSFOSMutex()
    {
        pthread_mutexattr_t mutexAttributes;

        int error;

        error = pthread_mutexattr_init(&mutexAttributes);
        if (error != 0)
        {
            throw std::runtime_error("Mutex pthread_mutexattr_init() failed in constructor: " + toString(strerror(error)));
        }

        error = pthread_mutexattr_settype(&mutexAttributes, PTHREAD_MUTEX_RECURSIVE);
        if (error != 0)
        {
            throw std::runtime_error("Mutex pthread_mutexattr_settype() failed in constructor: " + toString(strerror(error)));
        }

        error = pthread_mutex_init(&mutex, &mutexAttributes);
        if (error != 0)
        {
            throw std::runtime_error("Mutex pthread_mutex_init() failed in constructor: " + toString(strerror(error)));
        }
    }

    NSFOSMutex_POSIX::~NSFOSMutex_POSIX()
    {
        pthread_mutex_destroy(&mutex);
    }

    void NSFOSMutex_POSIX::lock()
    {
        int error = pthread_mutex_lock(&mutex);
        if (error != 0)
        {
            throw std::runtime_error("Mutex pthread_mutex_lock() failed in lock(): " + toString(strerror(error)));
        }
    }

    void NSFOSMutex_POSIX::unlock()
    {
        int error = pthread_mutex_unlock(&mutex);
        if (error != 0)
        {
            throw std::runtime_error("Mutex pthread_mutex_unlock() failed in unlock(): " + toString(strerror(error)));
        }
    }
}

#endif // NSF_OS_POSIX

