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

#ifndef NSF_OS_THREAD_POSIX_H
#define NSF_OS_THREAD_POSIX_H

#include "NSFOSThread.h"

#include <pthread.h>
#include <semaphore.h>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system thread in the POSIX environment.
    /// </summary>
    class NSFOSThread_POSIX : public NSFOSThread
    {
    public:

        /// <summary>
        /// Creates an operating system thread in the POSIX environment.
        /// </summary>
        /// <param name="name">The name for the thread.</param>
        /// <param name="executionAction">The action executed by the thread.</param>
        /// <returns>The new thread.</returns>
        /// <remarks>
        /// The thread execution action is typically an execution loop.  When the action returns, the thread is terminated.
        /// </remarks>
        NSFOSThread_POSIX(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction);

        /// <summary>
        /// Creates an operating system thread in the POSIX environment.
        /// </summary>
        /// <param name="name">The name for the thread.</param>
        /// <param name="executionAction">The action executed by the thread.</param>
        /// <param name="priority">The priority of the thread.</param>
        /// <returns>The new thread.</returns>
        /// <remarks>
        /// The thread execution action is typically an execution loop.  When the action returns, the thread is terminated.
        /// </remarks>
        NSFOSThread_POSIX(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority);

        /// <summary>
        /// Destroys an operating system thread in the POSIX environment.
        /// </summary>
        ~NSFOSThread_POSIX();

        virtual int getPriority() const;

        virtual void setPriority(int priority);

        virtual int getOSThreadId();

        virtual void startThread();

        static void sleepUntil (timespec* timeToSleep);

    private:

        sem_t startSemaphore;
        pthread_attr_t threadAttributes;
        sched_param threadScheduleParameter;
        pthread_t thread;

        // Stack size based on testing examples in Win32 environment
        // End-users are advised to test and adjust as necessary
        static const size_t StackSize = 0x6000;
        unsigned char stack[StackSize];

        /// <summary>
        /// Performs common construction behaviors.
        /// </summary>
        void construct(int priority);

        /// <summary>
        /// Thread entry point.
        /// </summary>
        static void* threadEntry(void* thread);
    };
}

#endif // NSF_OS_THREAD_POSIX_H

