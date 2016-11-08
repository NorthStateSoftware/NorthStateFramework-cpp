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

#include "ContextSwitchTest.h"


namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to member for action invocation, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    ContextSwitchTest::ContextSwitchTest(const NSFString& name)
        : name(name.c_str()), contextSwitchCount(0),
        signal1(NSFOSSignal::create("Signal1")), signal2(NSFOSSignal::create("Signal2")),
        thread1(NSFOSThread::create("Thread1", NSFAction(this, &ContextSwitchTest::thread1Loop), NSFOSThread::getHighestPriority())),
        thread2(NSFOSThread::create("Thread2", NSFAction(this, &ContextSwitchTest::thread2Loop), NSFOSThread::getHighestPriority()))
    {
    }

    bool ContextSwitchTest::runTest(NSFString& errorMessage)
    {
        thread1->startThread();
        thread2->startThread();

        NSFTime startTime = NSFTimerThread::getPrimaryTimerThread().getCurrentTime();

        while (contextSwitchCount < TotalContextSwitches)
        {
            NSFOSThread::sleep(1);
        }

        NSFTime endTime = NSFTimerThread::getPrimaryTimerThread().getCurrentTime();

        NSFTime contextSwitchTime = ((endTime - startTime) * 1000) / TotalContextSwitches;

        // Add results to name for test visibility
        name += "; Switch Time = " + toString(contextSwitchTime) + " uS";

        return true;
    }


    // Private

    void ContextSwitchTest::thread1Loop(const NSFContext& context)
    {
        while (contextSwitchCount < TotalContextSwitches)
        {
            signal1->send();
            signal2->wait(1000);
            contextSwitchCount += 2;
        }
    }

    void ContextSwitchTest::thread2Loop(const NSFContext& context)
    {
        while (contextSwitchCount < TotalContextSwitches)
        {
            signal1->wait(1000);
            signal2->send();
        }
    }


}
