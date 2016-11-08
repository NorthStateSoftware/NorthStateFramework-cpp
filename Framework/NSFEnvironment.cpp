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
