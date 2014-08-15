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

#include "TimerGetTimeTest.h"

using namespace NorthStateFramework;

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    TimerGetTimeTest::TimerGetTimeTest(const NSFString& name)
        : name(name.c_str())
    {
    }

    bool TimerGetTimeTest::runTest(NSFString& errorMessage)
    {
        int loops = 100000;
        NSFTime endTime;
        NSFTime startTime = NSFTimerThread::getPrimaryTimerThread().getCurrentTime();

        for (int i = 0; i < loops; ++i)
        {
            endTime = NSFTimerThread::getPrimaryTimerThread().getCurrentTime();
        }

        NSFTime getTime = ((endTime - startTime) * 1000000) / loops;

        // Add results to name for test visibility
        name += "; Get Time = " + toString(getTime) + " nS";

        return true;
    }
}