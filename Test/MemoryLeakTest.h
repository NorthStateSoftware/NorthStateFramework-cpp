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

#ifndef MEMORY_LEAK_TEST_H
#define MEMORY_LEAK_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test for increase of memory usage
    /// </summary>
    class MemoryLeakTest : public ITestInterface
    {
    public:
        MemoryLeakTest(ITestInterface* testToRepeat, int numberOfCycles);

        const NSFString& getName() { return name;}

        bool runTest(NSFString& errorMessage);

    private:
        NSFString name;
        ITestInterface* testToRepeat;
        int numberOfCycles;
    };
}

#endif // MEMORY_LEAK_TEST_H