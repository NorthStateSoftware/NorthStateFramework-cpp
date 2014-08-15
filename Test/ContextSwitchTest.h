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

#ifndef CONTEXT_SWITCH_TEST_H
#define CONTEXT_SWITCH_TEST_H

#include "TestHarness.h"
#include "TestInterface.h"

using namespace NorthStateFramework;

namespace NSFTest
{
    /// <summary>
    /// Test the context switch time
    /// </summary>
    class ContextSwitchTest :  public ITestInterface
    {
    public:

        ContextSwitchTest(const NSFString& name);

        const NSFString& getName() { return name; }

        bool runTest(NSFString& errorMessage);

    private:

        NSFString name;

        int contextSwitchCount;

        NSFOSSignal* signal1;
        NSFOSSignal* signal2;

        NSFOSThread* thread1;
        NSFOSThread* thread2;

        static const int TotalContextSwitches = 100000;

        void thread1Loop(const NSFContext& context);
        void thread2Loop(const NSFContext& context);
    };
}

#endif //CONTEXT_SWITCH_TEST_H
