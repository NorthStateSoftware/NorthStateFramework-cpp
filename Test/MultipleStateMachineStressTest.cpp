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

#include "MultipleStateMachineStressTest.h"

#include "ContinuouslyRunningTest.h"

using namespace NorthStateFramework;


namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    MultipleStateMachineStressTest::MultipleStateMachineStressTest(const NSFString& name, int numberOfInstances, int numberOfCycles)
        :name(name.c_str()), numberOfCycles(numberOfCycles), numberOfInstances(numberOfInstances), readyToTerminate(false)
    {
    }

    bool MultipleStateMachineStressTest::runTest(NSFString& errorMessage)
    {
        bool returnValue = true;
        readyToTerminate = false;

        std::list<ContinuouslyRunningTest*> tests;
        ContinuouslyRunningTest* firstTest;        

        for (int i = 0; i < numberOfInstances; ++i)
        {
            ContinuouslyRunningTest* test = new ContinuouslyRunningTest(getName() + ".SubTest" + toString(i), numberOfCycles);
            if (i == 0)
            {
                firstTest = test;
                test->getState1_2_2()->EntryActions += NSFAction(this, &MultipleStateMachineStressTest::terminateTest);
            }

            tests.push_back(test);

            test->runTest(errorMessage);
        }

        while (!readyToTerminate)
        {
            NSFOSThread::sleep(1);
        }

        // Terminate all the tests
        std::list<ContinuouslyRunningTest*>::iterator testIterator;
        for (testIterator = tests.begin(); testIterator != tests.end(); ++testIterator)
        {
            (*testIterator)->terminate(false);
        }

        // Wait for all tests to terminate
        for (testIterator = tests.begin(); testIterator != tests.end(); ++testIterator)
        {
            (*testIterator)->terminate(true);
        }

        firstTest->getState1_2_2()->EntryActions -= NSFAction(this, &MultipleStateMachineStressTest::terminateTest);

        // Delete all the tests
        for (testIterator = tests.begin(); testIterator != tests.end(); ++testIterator)
        {
            delete (*testIterator);
        }

        NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole("");

        return returnValue;
    }

    void MultipleStateMachineStressTest::terminateTest(const NSFStateMachineContext&)
    {
        readyToTerminate = true;
    }
}
