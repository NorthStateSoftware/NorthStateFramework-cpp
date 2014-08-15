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

#ifndef NSF_OS_SIGNAL_H
#define NSF_OS_SIGNAL_H

#include "NSFTimerAction.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system signal that may be used to block thread execution until the signal is sent.
    /// </summary>
    class NSFOSSignal : public NSFTimerAction
    {
    public:

        /// <summary>
        /// Creates an operating system signal.
        /// </summary>
        /// <param name="name">The name of the signal.</param>
        static NSFOSSignal* create(const NSFString& name);

        /// <summary>
        /// Clears the signal.
        /// </summary>
        /// <remarks>
        /// This method sets the signal to a non-signaled state,
        /// so that the next wait will block until a send is called.
        /// </remarks>
        virtual void clear() = 0;

        /// <summary>
        /// Sends the signal.
        /// </summary>
        virtual void send() = 0;

        /// <summary>
        /// Waits for signal to be sent.
        /// </summary>
        /// <returns>True if the signal was sent, false otherwise.</returns>
        /// <remarks>
        /// This method will block the calling thread until the signal is sent.
        /// Multiple threads must not wait on the same signal.
        /// </remarks>
        virtual bool wait() = 0;

        /// <summary>
        /// Waits for up to <paramref name="timeout"/> milliseconds for a signal to be sent.
        /// </summary>
        /// <param name="timeout">The maximum number of milliseconds to wait.</param>
        /// <returns>True if the signal was sent, false otherwise.</returns>
        /// <remarks>
        /// This method will block the calling thread until the signal is sent or the timeout occurs.
        /// Multiple threads must not wait on the same signal.
        /// </remarks>
        virtual bool wait(Int32 timeout) = 0;

    protected:

        /// <summary>
        /// Creates an operating system signal.
        /// </summary>
        /// <param name="name">The name of the signal.</param>
        NSFOSSignal(const NSFString& name);

    private:

        /// <summary>
        /// Callback method supporting NSFTimerAction interface.
        /// </summary>
        /// <remarks>
        /// This method is called by the NSFTimerThread to send the signal at the scheduled time.
        /// </remarks>
        virtual void execute();
    };
}

#endif // NSF_OS_SIGNAL_H
