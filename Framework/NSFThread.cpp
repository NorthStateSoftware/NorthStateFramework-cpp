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

#include "NSFThread.h"

#include "NSFEnvironment.h"
#include "NSFExceptionHandler.h"
#include "NSFOSThread.h"
#include "NSFTimerThread.h"

namespace NorthStateFramework
{
    int NSFThread::terminationSleepTime = 10;
    int NSFThread::terminationTimeout = 60000;

    // Public

    NSFThread::~NSFThread()
    {
        terminate(true);
        delete osThread;
        delete threadMutex;
    }

    void NSFThread::terminate(bool waitForTerminated)
    {
        LOCK(getThreadMutex())
        {
            if (terminationStatus != ThreadTerminated)
            {
                terminationStatus = ThreadTerminating;
            }
        }
        ENDLOCK;

        if (waitForTerminated)
        {
            for (int i = 0; i < terminationTimeout; i += terminationSleepTime)
            {
                if (terminationStatus == ThreadTerminated)
                {
                    return;
                }

                NSFOSThread::sleep(terminationSleepTime);
            }
            handleException(std::runtime_error(NSFString("Thread was unable to terminate")));
        }
    }

    // Protected

    NSFOSMutex* NSFThread::getThreadMutex()
    {
        return threadMutex;
    }

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to member for action invocation, disable warning as this is perfectly safe.
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif

    NSFThread::NSFThread(const NSFString& name)
        : NSFTaggedObject(name),
        osThread(NSFOSThread::create(name, NSFAction(this, &NSFThread::threadEntry), NSFOSThread::getMediumPriority())), threadMutex(NSFOSMutex::create()),
        terminationStatus(ThreadReady)
    {
        NSFEnvironment::getEnvironment().addThread(this);
    }

    NSFThread::NSFThread(const NSFString& name, int priority)
        : NSFTaggedObject(name),
        osThread(NSFOSThread::create(name, NSFAction(this, &NSFThread::threadEntry), priority)), threadMutex(NSFOSMutex::create()),
        terminationStatus(ThreadReady)
    {
        NSFEnvironment::getEnvironment().addThread(this);
    }

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to member for action invocation, disable warning as this is perfectly safe.
#pragma warning( pop )
#endif

    void NSFThread::handleException(const std::exception& exception)
    {
        // NSFExceptionContext maintains exception as a reference, so use two statements for proper scoping
        std::runtime_error newException(getName() + " thread exception: " + exception.what());
        NSFExceptionContext newContext(this, newException);

        ExceptionActions.execute(newContext);
        NSFExceptionHandler::getExceptionHandler().handleException(newContext);
    }

    void NSFThread::startThread()
    {
        osThread->startThread();
    }

    // Private

    void NSFThread::threadEntry(const NSFContext&)
    {
        try
        {
            threadLoop();
        }
        catch(const std::exception& exception)
        {
            std::runtime_error newException(getName() + " thread loop exception: " + exception.what());
            handleException(newException);
        }
        catch(...)
        {
            std::runtime_error newException(getName() + " thread loop exception: unknown exception");
            handleException(newException);
        }

        LOCK(getThreadMutex())
        {
            NSFEnvironment::getEnvironment().removeThread(this);
            terminationStatus = ThreadTerminated;
        }
        ENDLOCK;
    }
}
