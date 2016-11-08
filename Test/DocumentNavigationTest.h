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

#ifndef DOCUMENT_NAVIGATION_TEST_H
#define DOCUMENT_NAVIGATION_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;
using namespace std;

namespace NSFTest
{
    /// <summary>
    /// Test NSFDocument loading and navigation.
    /// </summary>
    class DocumentNavigationTest : public ITestInterface
    {
    public:
        DocumentNavigationTest(const NSFString& name, string fileName);

        const NSFString& getName() { return name;}

        bool runTest(NSFString& errorMessage);

    private:
        NSFString name;
        NSFString fileName;
    };
}

#endif // DOCUMENT_NAVIGATION_TEST_H