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

#ifndef NSF_OS_SIGNAL_H
#define NSF_OS_SIGNAL_H

#include "NSFTimerAction.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system signal that may be used to block thread execution until the signal is sent.
    /// </summary>
    class NSFOSSignal : public NSFTimerAction
    {
    public:

        /// <summary>
        /// Creates an operating system signal.
        /// </summary>
        /// <param name="name">The name of the signal.</param>
        static NSFOSSignal* create(const NSFString& name);

        /// <summary>
        /// Clears the signal.
        /// </summary>
        /// <remarks>
        /// This method sets the signal to a non-signaled state,
        /// so that the next wait will block until a send is called.
        /// </remarks>
        virtual void clear() = 0;

        /// <summary>
        /// Sends the signal.
        /// </summary>
        virtual void send() = 0;

        /// <summary>
        /// Waits for signal to be sent.
        /// </summary>
        /// <returns>True if the signal was sent, false otherwise.</returns>
        /// <remarks>
        /// This method will block the calling thread until the signal is sent.
        /// Multiple threads must not wait on the same signal.
        /// </remarks>
        virtual bool wait() = 0;

        /// <summary>
        /// Waits for up to <paramref name="timeout"/> milliseconds for a signal to be sent.
        /// </summary>
        /// <param name="timeout">The maximum number of milliseconds to wait.</param>
        /// <returns>True if the signal was sent, false otherwise.</returns>
        /// <remarks>
        /// This method will block the calling thread until the signal is sent or the timeout occurs.
        /// Multiple threads must not wait on the same signal.
        /// </remarks>
        virtual bool wait(Int32 timeout) = 0;

    protected:

        /// <summary>
        /// Creates an operating system signal.
        /// </summary>
        /// <param name="name">The name of the signal.</param>
        NSFOSSignal(const NSFString& name);

    private:

        /// <summary>
        /// Callback method supporting NSFTimerAction interface.
        /// </summary>
        /// <remarks>
        /// This method is called by the NSFTimerThread to send the signal at the scheduled time.
        /// </remarks>
        virtual void execute();
    };
}

#endif // NSF_OS_SIGNAL_H
