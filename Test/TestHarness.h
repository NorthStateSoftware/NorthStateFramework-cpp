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

#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include "NorthStateFramework.h"

namespace NSFTest
{
    class TestHarness 
    {
    private:

        static const int DefaultWaitTime = 1000;
        NSFOSSignal* firstSignal;
        NSFOSSignal* secondSignal;

    public: 

        TestHarness();

        ~TestHarness();

        /// <summary>
        /// Determines if the queuing the an event causes a state to enter before DefaultWaitTime elapses.
        /// </summary>
        /// <param name="eventToInject">The event to queue.</param>
        /// <param name="stateToWaitFor">The state to wait for.</param>
        /// <returns>
        ///     True if the stateToWaitFor is entered.
        ///     False if the stateToWaitFor is not entered before time expires.
        /// </returns>
        /// <remarks>
        /// This is a blocking call, the caller may be blocked for as long as <see cref="DefaultWaitTime"/>.
        /// </remarks>
        bool doesEventResultInState(NSFEvent* eventToInject, NSFState* stateToWaitFor);

        /// <summary>
        /// Determines if the queuing the an event causes a state to enter before DefaultWaitTime elapses.
        /// </summary>
        /// <param name="eventToInject">The event to queue.</param>
        /// <param name="stateToWaitFor">The state to wait for.</param>
        /// <param name="waitTime">The time in ms to wait for the stateToWaitFor to enter.</param>
        /// <returns>
        ///     True if the stateToWaitFor is entered.
        ///     False if the stateToWaitFor is not entered before time expires.
        /// </returns>
        /// <remarks>
        /// This is a blocking call, the caller may be blocked for as long as waitTime.
        /// </remarks>
        bool doesEventResultInState(NSFEvent* eventToInject, NSFState* stateToWaitFor, int waitTime);

        /// <summary>
        /// Determines if the queuing the an event causes two states to enter before DefaultWaitTime elapses.
        /// </summary>
        /// <param name="eventToInject">The event to queue.</param>
        /// <param name="stateToWaitFor">The first of two states to wait for.</param>
        /// <param name="secondStateToWaitFor">The second of two states to wait for.</param>
        /// <returns>
        ///     True if both states are entered.
        ///     False if either of the states fails to enter before time expires.
        /// </returns>
        /// <remarks>
        /// This is a blocking call, the caller may be blocked for as long as twice <see cref="DefaultWaitTime"/>.
        /// The states may be congruently entered.
        /// </remarks>
        bool doesEventResultInState(NSFEvent* eventToInject, NSFState* stateToWaitFor, NSFState* secondStateToWaitFor);


        /// <summary>
        /// Determines if the queuing the an event causes two states to enter before waitTime elapses.
        /// </summary>
        /// <param name="eventToInject">The event to queue.</param>
        /// <param name="stateToWaitFor">The first of two states to wait for.</param>
        /// <param name="secondStateToWaitFor">The second of two states to wait for.</param>
        /// <param name="waitTime">The time in ms to wait for the each state to enter.</param>
        /// <returns>
        ///     True if both states are entered.
        ///     False if either of the states fails to enter before time expires.
        /// </returns>
        /// <remarks>
        /// This is a blocking call, the caller may be blocked for as long as twice waitTime.
        /// The states may be congruently entered.
        /// </remarks>
        bool doesEventResultInState(NSFEvent* eventToInject, NSFState* stateToWaitFor, NSFState* secondStateToWaitFor, int waitTime);
    };
}

#endif //TEST_HARNESS_H