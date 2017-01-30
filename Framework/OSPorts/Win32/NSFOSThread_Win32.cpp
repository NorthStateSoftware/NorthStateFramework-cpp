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

#if defined NSF_OS_WIN32

#include "NSFOSThread_Win32.h"

namespace NorthStateFramework
{
    // Base class definitions

    int NSFOSThread::getHighestPriority()
    {
        return THREAD_PRIORITY_HIGHEST;
    }

    int NSFOSThread::getHighPriority()
    {
        return THREAD_PRIORITY_ABOVE_NORMAL;
    }

    int NSFOSThread::getMediumPriority()
    {
        return THREAD_PRIORITY_NORMAL;
    }

    int NSFOSThread::getLowPriority()
    {
        return THREAD_PRIORITY_BELOW_NORMAL;
    }

    int NSFOSThread::getLowestPriority()
    {
        return THREAD_PRIORITY_LOWEST;
    }

    NSFOSThread* NSFOSThread::create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority)
    {
        return new NSFOSThread_Win32(name, executionAction, priority);
    }    

    NSFOSThread* NSFOSThread::create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
    {
        return new NSFOSThread_Win32(name, executionAction);
    }    

    void NSFOSThread::sleep(Int32 sleepTime)
    {
        if (sleepTime < 0)
        {
            sleepTime = 0;
        }

        ::Sleep(sleepTime);
    }

    // Concrete class definitions

    // Public

    NSFOSThread_Win32::NSFOSThread_Win32(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
        : NSFOSThread(name, executionAction), threadHandle(NULL), threadId(0)
    {
        construct(getMediumPriority());
    }

    NSFOSThread_Win32::NSFOSThread_Win32(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority)
        : NSFOSThread(name, executionAction), threadHandle(NULL), threadId(0)
    {
        construct(priority);
    }

    NSFOSThread_Win32::~NSFOSThread_Win32()
    {
        if (threadHandle != NULL)
        {
            CloseHandle(threadHandle);
        }
    }

    int NSFOSThread_Win32::getPriority() const
    {
        return priority;
    }

    void NSFOSThread_Win32::setPriority(int priority)
    {
        this->priority = priority;
        SetThreadPriority(threadHandle, priority);
    }

    int NSFOSThread_Win32::getOSThreadId()
    {
        return threadId;
    }

    void NSFOSThread_Win32::startThread()
    {
        ResumeThread(threadHandle);
    }

    // Private

    void NSFOSThread_Win32::construct(int priority)
    {
        threadHandle = CreateThread(NULL, NULL, &NSFOSThread_Win32::threadEntry, this, CREATE_SUSPENDED, &threadId);

        if (threadHandle == NULL)
        {
            throw std::runtime_error("Thread creation error: " + getName());
        }

        setPriority(priority);
    }

    DWORD WINAPI NSFOSThread_Win32::threadEntry(LPVOID thread)
    {
        ((NSFOSThread*)thread)->executeAction();

        return 0;
    }
}

#endif // NSF_OS_WIN32

