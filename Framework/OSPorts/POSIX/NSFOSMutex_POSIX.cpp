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

