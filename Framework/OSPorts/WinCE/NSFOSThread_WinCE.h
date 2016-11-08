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

#ifndef NSF_OS_THREAD_WINCE_H
#define NSF_OS_THREAD_WINCE_H

#include "NSFOSThread.h"

#include <windows.h>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system thread in the WinCE environment.
    /// </summary>
    class NSFOSThread_WinCE : public NSFOSThread
    {
    public:

        /// <summary>
        /// Creates an operating system thread in the WinCE environment.
        /// </summary>
        /// <param name="name">The name for the thread.</param>
        /// <param name="executionAction">The action executed by the thread.</param>
        /// <returns>The new thread.</returns>
        /// <remarks>
        /// The thread execution action is typically an execution loop.  When the action returns, the thread is terminated.
        /// </remarks>
        NSFOSThread_WinCE(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction);

        /// <summary>
        /// Creates an operating system thread in the WinCE environment.
        /// </summary>
        /// <param name="name">The name for the thread.</param>
        /// <param name="executionAction">The action executed by the thread.</param>
        /// <param name="priority">The priority of the thread.</param>
        /// <returns>The new thread.</returns>
        /// <remarks>
        /// The thread execution action is typically an execution loop.  When the action returns, the thread is terminated.
        /// </remarks>
        NSFOSThread_WinCE(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority);

        /// <summary>
        /// Destroys an operating system thread in the WinCE environment.
        /// </summary>
        ~NSFOSThread_WinCE();

        virtual int getPriority() const;

        virtual void setPriority(int priority);

        virtual int getOSThreadId();

        virtual void startThread();

    private:

        int priority;
        HANDLE threadHandle;
        DWORD threadId;

        /// <summary>
        /// Performs common construction behaviors.
        /// </summary>
        void construct(int priority);

        /// <summary>
        /// Thread entry point.
        /// </summary>
        static DWORD WINAPI threadEntry(LPVOID thread);
    };
}

#endif // NSF_OS_THREAD_WINCE_H

