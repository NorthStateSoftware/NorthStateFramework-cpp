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

