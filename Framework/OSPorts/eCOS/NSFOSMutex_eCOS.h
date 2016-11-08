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

#ifndef NSF_OS_MUTEX_ECOS_H
#define NSF_OS_MUTEX_ECOS_H

#include "NSFOSMutex.h"

#include <cyg/kernel/kapi.h>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system mutex in the eCOS environment.
    /// </summary>
    class NSFOSMutex_eCOS : public NSFOSMutex
    {
    public:

        /// <summary>
        /// Creates an operating system mutex in the eCOS environment.
        /// </summary>
        NSFOSMutex_eCOS();

        /// <summary>
        /// Destroys an operating system mutex in the eCOS environment.
        /// </summary>
        ~NSFOSMutex_eCOS();

        void lock();

        void unlock();

    private:

        cyg_mutex_t mutex;
    };
}

#endif // NSF_OS_MUTEX_ECOS_H

