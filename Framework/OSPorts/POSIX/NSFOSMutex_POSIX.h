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

#ifndef NSF_OS_MUTEX_POSIX_H
#define NSF_OS_MUTEX_POSIX_H

#include "NSFOSMutex.h"

#include <pthread.h>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system mutex in the POSIX environment.
    /// </summary>
    class NSFOSMutex_POSIX : public NSFOSMutex
    {
    public:

        friend class NSFOSSignal_POSIX;

        /// <summary>
        /// Creates an operating system mutex in the POSIX environment.
        /// </summary>
        NSFOSMutex_POSIX();

        /// <summary>
        /// Destroys an operating system mutex in the POSIX environment.
        /// </summary>
        ~NSFOSMutex_POSIX();

        void lock();

        void unlock();

    private:

        pthread_mutex_t mutex;
    };
}

#endif // NSF_OS_MUTEX_POSIX_H

