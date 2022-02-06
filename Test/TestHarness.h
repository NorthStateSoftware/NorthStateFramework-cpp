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