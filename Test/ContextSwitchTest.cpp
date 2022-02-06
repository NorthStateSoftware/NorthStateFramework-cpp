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

    bool ContextSwitchTest::runTest(NSFString&)
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

    void ContextSwitchTest::thread1Loop(const NSFContext&)
    {
        while (contextSwitchCount < TotalContextSwitches)
        {
            signal1->send();
            signal2->wait(1000);
            contextSwitchCount += 2;
        }
    }

    void ContextSwitchTest::thread2Loop(const NSFContext&)
    {
        while (contextSwitchCount < TotalContextSwitches)
        {
            signal1->wait(1000);
            signal2->send();
        }
    }


}
