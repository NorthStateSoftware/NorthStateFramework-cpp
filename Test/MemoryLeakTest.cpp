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

#include "MemoryLeakTest.h"
#if defined WIN32
#include <windows.h>
#include <psapi.h>
#endif

using namespace NorthStateFramework;

namespace NSFTest
{

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    MemoryLeakTest::MemoryLeakTest(ITestInterface* testToRepeat, int numberOfCycles)
    {
        this->name = "Memory Leak Test - " + testToRepeat->getName();
        this->testToRepeat = testToRepeat;
        this->numberOfCycles = numberOfCycles;
    }

    bool MemoryLeakTest::runTest(NSFString& errorMessage)
    {
        bool returnValue = true;

        while (!NSFTraceLog::getPrimaryTraceLog().allTracesLogged())
        {
            NSFOSThread::sleep(1);
        }

#if defined WIN32
        HANDLE hProc = GetCurrentProcess();
        PROCESS_MEMORY_COUNTERS_EX infoStart;
        PROCESS_MEMORY_COUNTERS_EX infoEnd;
        infoStart.cb = sizeof(infoStart);
        infoEnd.cb = sizeof(infoEnd);
        //Read the memory info before starting the test ... this will preload any statics for that test
        GetProcessMemoryInfo(hProc, (PROCESS_MEMORY_COUNTERS*)&infoStart, infoStart.cb);
#endif

        // Run the test once ... this will preload any statics
        testToRepeat->runTest(errorMessage);

        // Give some time for everything to clean up
        while (!NSFTraceLog::getPrimaryTraceLog().allTracesLogged())
        {
            NSFOSThread::sleep(1);
        }

#if defined WIN32
        // Read the memory info at the start of the test
        GetProcessMemoryInfo(hProc, (PROCESS_MEMORY_COUNTERS*)&infoStart, infoStart.cb);
#endif

        // Run the test over and over again ...
        for (int i = 0; i < numberOfCycles; i++)
        {
            testToRepeat->runTest(errorMessage);
        }

        // Give some time for everything to clean up
        while (!NSFTraceLog::getPrimaryTraceLog().allTracesLogged())
        {
            NSFOSThread::sleep(1);
        }

#if defined WIN32
        // Read the memory info at the end of the test.
        GetProcessMemoryInfo(hProc, (PROCESS_MEMORY_COUNTERS*)&infoEnd, infoEnd.cb);


        long startingKB = infoStart.PrivateUsage / 1024;
        long endingKB = infoEnd.PrivateUsage / 1024;

        if (endingKB > startingKB)
        {
            long delta = endingKB - startingKB;
            int percentChange = (int)((delta * 100 )/ startingKB);
            if (percentChange >= 2)
            {
                errorMessage = "Encrease in memory detected in " 
                    + testToRepeat->getName() 
                    + " Starting memory: " + toString<unsigned long>(startingKB) 
                    + ", Ending memory: " + toString<unsigned long>(endingKB) 
                    + ", Delta: " + toString<unsigned long>(delta) + " KB, " + toString<unsigned long>(percentChange) + " %";

                returnValue = false;
            }
        }
        else
        {
            NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("PrivateUsage Mem: Start: ") + toString<DWORD>(startingKB));
            NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("PrivateUsage Mem: End: ") + toString<DWORD>(endingKB));
        }
#endif

        return returnValue;
    }

}