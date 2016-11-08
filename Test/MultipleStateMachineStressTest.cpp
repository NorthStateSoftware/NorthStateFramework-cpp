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
        :name(name.c_str()), readyToTerminate(false), numberOfCycles(numberOfCycles), numberOfInstances(numberOfInstances)
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

    void MultipleStateMachineStressTest::terminateTest(const NSFStateMachineContext& context)
    {
        readyToTerminate = true;
    }
}
