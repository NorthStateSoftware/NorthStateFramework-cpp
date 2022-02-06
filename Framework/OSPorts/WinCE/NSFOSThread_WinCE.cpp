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

