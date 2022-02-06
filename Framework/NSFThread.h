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

#ifndef NSF_THREAD_H
#define NSF_THREAD_H

#include "NSFDelegates.h"
#include "NSFOSTypes.h"
#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents the termination status of a thread.
    /// </summary>
    enum NSFThreadTerminationStatus{ThreadReady = 1, ThreadTerminating, ThreadTerminated};

    /// <summary>
    /// Represents the common framework thread functionality.
    /// </summary>
    class NSFThread : public NSFTaggedObject
    {
    public:

        /// <summary>
        /// Destroys an event thread.
        /// </summary>
        /// <remarks>
        /// This destructor will block for a short period until the thread is terminated.
        /// </remarks>
        virtual ~NSFThread();

        /// <summary>
        /// Actions to execute if an exception is encountered while the thread is executing.
        /// </summary>
        NSFVoidActions<NSFExceptionContext> ExceptionActions;

        /// <summary>
        /// Gets the underlying OS thread.
        /// </summary>
        NSFOSThread* getOSThread() const { return osThread; }

        /// <summary>
        /// Gets the thread termination status.
        /// </summary>
        NSFThreadTerminationStatus getTerminationStatus() const { return terminationStatus; }

        /// <summary>
        /// Gets the amount of time (mS) the <see cref="terminate"/> method sleeps between checks
        /// for event handler termination.
        /// </summary>
        int getTerminationSleepTime() { return terminationSleepTime; }

        /// <summary>
        /// Sets the amount of time (mS) the <see cref="terminate"/> method sleeps between checks
        /// for event handler termination.
        /// </summary>
        void setTerminationSleepTime(int value) { terminationSleepTime = value; }

        /// <summary>
        /// Gets the amount of time the <see cref="terminate"/> method will wait for event processing to complete.
        /// </summary>
        static int getTerminationTimeout() { return terminationTimeout; }

        /// <summary>
        /// Sets the amount of time the <see cref="terminate"/> method will wait for event processing to complete.
        /// </summary>
        static void setTerminationTimeout(int value) { terminationTimeout = value; }

        /// <summary>
        /// Terminates the thread by causing the execution method to return.
        /// </summary>
        /// <param name="waitForTerminated">Flag indicating if the method should wait until the thread is terminated (true), or if it should return immediately (false).</param>
        /// <remarks>
        /// This method does not have to be called before deleting a thread,
        /// because the proper termination sequence is guaranteed by the destructor.
        /// However, this method is useful to guarantee that the thread is no longer active.
        /// If the waitForTerminated flag is set true, this method must not be called from its thread of execution.
        /// </remarks>
        virtual void terminate(bool waitForTerminated);

    protected:

        /// <summary>
        /// Creates a thread.
        /// </summary>
        /// <param name="name">The name of the thread.</param>
        /// <remarks>
        /// The thread is created with normal priority.
        /// To change the priority, use the getOSThread() method to access the underlying thread object.
        /// </remarks>
        NSFThread(const NSFString& name);

        /// <summary>
        /// Creates an event thread.
        /// </summary>
        /// <param name="name">The name of the thread.</param>
        /// <param name="priority">The priority of the thread.</param>
        NSFThread(const NSFString& name, int priority);

        /// <summary>
        /// Gets the thread mutex.
        /// </summary>
        /// <remarks>
        /// This mutex is used by the framework to protect changes to thread status.
        /// </remarks>
        NSFOSMutex* getThreadMutex();

        /// <summary>
        /// Handles exceptions caught by main event processing loop.
        /// </summary>
        /// <param name="exception">The exception caught.</param>
        void handleException(const std::exception& exception);

        /// <summary>
        /// Starts the thread by calling its execution action.
        /// </summary>
        void startThread();

    private:

        NSFOSThread* osThread;
        NSFOSMutex* threadMutex;
        NSFThreadTerminationStatus terminationStatus;
        static int terminationSleepTime;
        static int terminationTimeout;

        /// <summary>
        /// Implements the entry point for the thread.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void threadEntry(const NSFContext& context);

        /// <summary>
        /// Implements the main thread processing loop.
        /// </summary>
        virtual void threadLoop() = 0;
    };
}

#endif // NSF_THREAD_H
