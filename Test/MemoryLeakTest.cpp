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