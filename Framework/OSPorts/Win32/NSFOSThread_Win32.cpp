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

