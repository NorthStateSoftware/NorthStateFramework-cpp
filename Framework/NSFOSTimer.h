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

#ifndef NSF_OS_TIMER_H
#define NSF_OS_TIMER_H

#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system timer that can be used to retrieve accurate time and block waiting for the next timeout.
    /// </summary>
    class NSFOSTimer : public NSFTaggedObject
    {
    public:

        /// <summary>
        /// Gets the current time in milliseconds since the timer was created.
        /// </summary>
        /// <returns>The current time in milliseconds.</returns>
        virtual NSFTime getCurrentTime() = 0;

        /// <summary>
        /// Creates an operating system timer.
        /// </summary>
        /// <param name="name">The name of the timer.</param>
        /// <returns>The new timer.</returns>
        static NSFOSTimer* create(const NSFString& name);

        /// <summary>
        /// Sets the next timeout.
        /// </summary>
        /// <param name="timeout">The absolute time of the next timeout in milliseconds.</param>
        /// <remarks>
        /// A thread blocked by the method waitForNextTimeTimeout() must unblock after the specified timeout.
        /// </remarks>
        virtual void setNextTimeout(NSFTime timeout) = 0;

        /// <summary>
        /// Waits for the next tick period.
        /// </summary>
        /// <remarks>
        /// This method shall block the calling thread until the timeout specified by setNextTimeout(...).
        /// </remarks>
        virtual void waitForNextTimeout() = 0;

    protected:

        /// <summary>
        /// Creates an operating system timer.
        /// </summary>
        /// <param name="name">The name of the timer.</param>
        NSFOSTimer(const NSFString& name);
    };
}

#endif // NSF_OS_TIMER_H
