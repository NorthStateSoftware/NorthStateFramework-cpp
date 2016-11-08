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

#include "NSFThread.h"

#include "NSFEnvironment.h"
#include "NSFExceptionHandler.h"
#include "NSFOSThread.h"
#include "NSFTimerThread.h"

namespace NorthStateFramework
{
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
#pragma warning( disable : 4355 )
#endif

    NSFThread::NSFThread(const NSFString& name)
        : NSFTaggedObject(name),
        osThread(NSFOSThread::create(name, NSFAction(this, &NSFThread::threadEntry), NSFOSThread::getMediumPriority())), threadMutex(NSFOSMutex::create()),
        terminationStatus(ThreadReady), terminationSleepTime(10)
    {
        NSFEnvironment::getEnvironment().addThread(this);
    }

    NSFThread::NSFThread(const NSFString& name, int priority)
        : NSFTaggedObject(name),
        osThread(NSFOSThread::create(name, NSFAction(this, &NSFThread::threadEntry), priority)), threadMutex(NSFOSMutex::create()),
        terminationStatus(ThreadReady), terminationSleepTime(10)
    {
        NSFEnvironment::getEnvironment().addThread(this);
    }

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

    void NSFThread::threadEntry(const NSFContext& context)
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
