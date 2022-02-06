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

#ifndef NSF_OS_TIMER_H
#define NSF_OS_TIMER_H

#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system timer that can be used to retrieve accurate time and block waiting for the next timeout.
    /// </summary>
    class NSFOSTimer : public NSFTaggedObject
    {
    public:

        /// <summary>
        /// Gets the current time in milliseconds since the timer was created.
        /// </summary>
        /// <returns>The current time in milliseconds.</returns>
        virtual NSFTime getCurrentTime() = 0;

        /// <summary>
        /// Creates an operating system timer.
        /// </summary>
        /// <param name="name">The name of the timer.</param>
        /// <returns>The new timer.</returns>
        static NSFOSTimer* create(const NSFString& name);

        /// <summary>
        /// Sets the next timeout.
        /// </summary>
        /// <param name="timeout">The absolute time of the next timeout in milliseconds.</param>
        /// <remarks>
        /// A thread blocked by the method waitForNextTimeTimeout() must unblock after the specified timeout.
        /// </remarks>
        virtual void setNextTimeout(NSFTime timeout) = 0;

        /// <summary>
        /// Waits for the next tick period.
        /// </summary>
        /// <remarks>
        /// This method shall block the calling thread until the timeout specified by setNextTimeout(...).
        /// </remarks>
        virtual void waitForNextTimeout() = 0;

    protected:

        /// <summary>
        /// Creates an operating system timer.
        /// </summary>
        /// <param name="name">The name of the timer.</param>
        NSFOSTimer(const NSFString& name);
    };
}

#endif // NSF_OS_TIMER_H
