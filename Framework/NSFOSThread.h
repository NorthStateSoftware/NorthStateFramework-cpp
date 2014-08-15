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

#ifndef NSF_OS_THREAD_H
#define NSF_OS_THREAD_H

#include "NSFDelegates.h"
#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system thread.
    /// </summary>
    class NSFOSThread : public NSFTaggedObject
    {
    public:

        /// <summary>
        /// Destroys an operating system thread.
        /// </summary>
        virtual ~NSFOSThread();

        /// <summary>
        /// Gets the value for the highest priority thread.
        /// </summary>
        /// <returns>The highest thread priority.</returns>
        /// <remarks>
        /// This methods returns the highest priority that an NSFOSThread may have.
        /// The system may support higher thread priorities which are reserved for system use.
        /// Only the NSFOSTimer should use this priority level.
        /// </remarks>
        static int getHighestPriority();

        /// <summary>
        /// Gets the value for a high priority thread.
        /// </summary>
        /// <returns>The highest thread priority.</returns>
        /// <remarks>
        /// Use setPriority to set a threads priority.
        /// </remarks>
        static int getHighPriority();

        /// <summary>
        /// Gets the value for a medium priority thread.
        /// </summary>
        /// <returns>The medium thread priority.</returns>
        /// <remarks>
        /// Use setPriority to set a threads priority.
        /// </remarks>
        static int getMediumPriority();

        /// <summary>
        /// Gets the value for a low priority thread.
        /// </summary>
        /// <returns>The low thread priority.</returns>
        /// <remarks>
        /// Use setPriority to set a threads priority.
        /// </remarks>
        static int getLowPriority();

        /// <summary>
        /// Gets the value for the lowest priority thread.
        /// </summary>
        /// <returns>The lowest thread priority.</returns>
        /// <remarks>
        /// This methods returns the lowest priority that an NSFOSThread may have.
        /// The system may support lower thread priorities which are reserved for system use.
        /// </remarks>
        static int getLowestPriority();

        /// <summary>
        /// Gets the priority of the thread.
        /// </summary>
        /// <returns>The priority of the thread.</returns>
        virtual int getPriority() const = 0;

        /// <summary>
        /// Sets the priority of the thread.
        /// </summary>
        /// <param name="priority">The priority of the thread.</param>
        virtual void setPriority(int priority) = 0;

        /// <summary>
        /// Gets the operating system specific thread id.
        /// </summary>
        /// <returns>The OS specific thread id.</returns>
        virtual int getOSThreadId() = 0;

        /// <summary>
        /// Creates an operating system thread.
        /// </summary>
        /// <param name="name">The name for the thread.</param>
        /// <param name="executionAction">The action executed by the thread.</param>
        /// <returns>The new thread.</returns>
        /// <remarks>
        /// The thread execution action is typically an execution loop.
        /// When the action returns, the thread is terminated.
        /// </remarks>
        static NSFOSThread* create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction);

        /// <summary>
        /// Creates an operating system thread.
        /// </summary>
        /// <param name="name">The name for the thread.</param>
        /// <param name="executionAction">The action executed by the thread.</param>
        /// <param name="priority">The priority of the thread.</param>
        /// <returns>The new thread.</returns>
        /// <remarks>
        /// The thread execution action is typically an execution loop.
        /// When the action returns, the thread is terminated.
        /// </remarks>
        static NSFOSThread* create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority);

        /// <summary>
        /// Executes the thread execution action.
        /// </summary>
        /// <remarks>
        /// The thread execution action is typically an execution loop.
        /// When the action returns, the thread is terminated.
        /// Derived classes should call this method from their thread entry method.
        /// This method should not be called in any other circumnstances.
        /// </remarks>
        void executeAction();

        /// <summary>
        /// Sleeps the calling thread for the specified number of milliseconds.
        /// </summary>
        /// <param name="sleepTime">Time to sleep in milliseconds.</param>
        /// <remarks>
        /// This method sleeps the calling thread.
        /// The thread will sleep for at least the specified number of millisecond, but no more than
        /// the specified number rounded up to the nearest clock period.
        /// A sleep time of zero has OS defined behavior.
        /// </remarks>
        static void sleep(UInt32 sleepTime);

        /// <summary>
        /// Starts the thread by calling its execution action.
        /// </summary>
        virtual void startThread() = 0;

    protected:

        /// <summary>
        /// Creates an operating system thread.
        /// </summary>
        /// <param name="name">The name for the thread.</param>
        /// <param name="executionAction">The execution action for the thread.</param>
        /// <remarks>
        /// The thread execution action is typically an execution loop.
        /// When the action returns, the thread is terminated.
        /// </remarks>
        NSFOSThread(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction);

        void handleException(const std::exception& exception);

    private:

        NSFVoidAction<NSFContext>* action;
    };
}

#endif // NSF_OS_THREAD_H

