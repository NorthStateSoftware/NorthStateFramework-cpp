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

#include "NSFCustomConfig.h"

#if defined NSF_OS_WINCE

#include "NSFOSThread_WinCE.h"

namespace NorthStateFramework
{
    // Base class definitions

    int NSFOSThread::getHighestPriority()
    {
#if defined WINCE
        return 1;  // Top of range for real-time above drivers
#elif defined WIN32  // For testing under Win32
        return THREAD_PRIORITY_HIGHEST;
#endif
    }

    int NSFOSThread::getHighPriority()
    {
#if defined WINCE
        return 48;  // Middle of range for real-time above drivers
#elif defined WIN32  // For testing under Win32
        return THREAD_PRIORITY_ABOVE_NORMAL;
#endif
    }

    int NSFOSThread::getMediumPriority()
    {
#if defined WINCE
        return 96;  // Bottom of range for real-time above drivers
#elif defined WIN32  // For testing under Win32
        return THREAD_PRIORITY_NORMAL;
#endif
    }

    int NSFOSThread::getLowPriority()
    {
#if defined WINCE
        return 254;  // One below lowest
#elif defined WIN32  // For testing under Win32
        return THREAD_PRIORITY_BELOW_NORMAL;
#endif
    }

    int NSFOSThread::getLowestPriority()
    {
#if defined WINCE
        return 255;  // Lowest possible in CE
#elif defined WIN32  // For testing under Win32
        return THREAD_PRIORITY_LOWEST;
#endif
    }

    NSFOSThread* NSFOSThread::create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority)
    {
        return new NSFOSThread_WinCE(name, executionAction, priority);
    }    

    NSFOSThread* NSFOSThread::create(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
    {
        return new NSFOSThread_WinCE(name, executionAction);
    }    

    void NSFOSThread::sleep(UInt32 sleepTime)
    {
        ::Sleep(sleepTime);
    }

    // Concrete class definitions

    // Public

    NSFOSThread_WinCE::NSFOSThread_WinCE(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
        : NSFOSThread(name, executionAction), threadHandle(NULL), threadId(0)
    {
        construct(getMediumPriority());
    }

    NSFOSThread_WinCE::NSFOSThread_WinCE(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction, int priority)
        : NSFOSThread(name, executionAction), threadHandle(NULL), threadId(0)
    {
        construct(priority);
    }

    NSFOSThread_WinCE::~NSFOSThread_WinCE()
    {
        if (threadHandle != NULL)
        {
            CloseHandle(threadHandle);
        }
    }

    int NSFOSThread_WinCE::getPriority() const
    {
        return priority;
    }

    void NSFOSThread_WinCE::setPriority(int priority)
    {
        this->priority = priority;

#if defined WINCE
        CeSetThreadPriority(threadHandle, priority);
#elif defined WIN32  // For testing under Win32
        SetThreadPriority(threadHandle, priority);    
#endif
    }

    int NSFOSThread_WinCE::getOSThreadId()
    {
        return threadId;
    }

    void NSFOSThread_WinCE::startThread()
    {
        ResumeThread(threadHandle);
    }

    // Private

    void NSFOSThread_WinCE::construct(int priority)
    {
        if (!(threadHandle = CreateThread(NULL, NULL, &NSFOSThread_WinCE::threadEntry, this, CREATE_SUSPENDED, &threadId)))
        {
            throw std::runtime_error("Thread creation error: " + getName());
        }

        setPriority(priority);
    }

    DWORD WINAPI NSFOSThread_WinCE::threadEntry(LPVOID thread)
    {
        ((NSFOSThread*)thread)->executeAction();

        return 0;
    }
}

#endif // NSF_OS_WINCE

