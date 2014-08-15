// Copyright 2004-2014, North State Software, LLC.  All rights reserved.

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

#ifndef THREAD_CREATION_TEST_H
#define THREAD_CREATION_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test Basic State Machine Behavior
    /// </summary>
    class ThreadCreationTest :  public ITestInterface
    {
    public:
        ThreadCreationTest(const NSFString& name);

        ~ThreadCreationTest();

        const NSFString& getName() { return thread->getName(); }

        bool runTest(NSFString& errorMessage);

    private:
        NSFEventThread* thread;
    };
}

#endif //THREAD_CREATION_TEST_H