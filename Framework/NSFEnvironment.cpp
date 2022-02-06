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

#include "NSFEnvironment.h"
#include "NSFTimerThread.h"
#include "NSFTraceLog.h"

namespace NorthStateFramework
{
    // Public

    std::list<NSFThread*> NSFEnvironment::getThreads()
    {
        LOCK(environmentMutex)
        {
            return threadList;
        }
        ENDLOCK;
    }

    void NSFEnvironment::start()
    {
        // Lazy instantiate threaded environment features in required order
        NSFTimerThread::getPrimaryTimerThread();
        NSFTraceLog::getPrimaryTraceLog();
    }

    // Private

    NSFEnvironment& NSFEnvironment::environment(getEnvironment());
    bool NSFEnvironment::autoStarted(getEnvironment().autoStart());

    NSFEnvironment::NSFEnvironment()
        : environmentMutex(NSFOSMutex::create())
    {
    }

    NSFEnvironment::~NSFEnvironment()
    {
        terminate();
        delete environmentMutex;
    }

    bool NSFEnvironment::autoStart()
    {
#if defined NSF_AUTO_START
        start();
        return true;
#else
        return false;
#endif
    }

    void NSFEnvironment::addThread(NSFThread* thread)
    {
        LOCK(environmentMutex)
        {
            threadList.push_back(thread);
        }
        ENDLOCK;
    }

    void NSFEnvironment::removeThread(NSFThread* thread)
    {
        LOCK(environmentMutex)
        {
            threadList.remove(thread);
        }
        ENDLOCK;
    }

    void NSFEnvironment::terminate()
    {
        // Get all thread terminations started
        const std::list<NSFThread*>& threads = getThreads();
        std::list<NSFThread*>::const_iterator threadIterator;
        for (threadIterator = threads.begin(); threadIterator != threads.end(); ++threadIterator)
        {
            (*threadIterator)->terminate(false);
        }

        // Wait for all threads to be terminated
        for (threadIterator = threads.begin(); threadIterator != threads.end(); ++threadIterator)
        {
            (*threadIterator)->terminate(true);
        }
    }
}
