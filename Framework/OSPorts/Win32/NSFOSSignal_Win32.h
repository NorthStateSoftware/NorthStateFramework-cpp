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

#ifndef NSF_OS_SIGNAL_WIN32_H
#define NSF_OS_SIGNAL_WIN32_H

#include "NSFOSSignal.h"

#include <windows.h>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system signal in the Win32 environment
    /// </summary>
    class NSFOSSignal_Win32 : public NSFOSSignal
    {
    public:

        /// <summary>
        /// Creates an operating system signal in the Win32 environment.
        /// </summary>
        /// <param name="name">The name of the signal.</param>
        NSFOSSignal_Win32(const NSFString& name);

        /// <summary>
        /// Destroys an operating system signal in the Win32 environment.
        /// </summary>
        ~NSFOSSignal_Win32();

        void clear();

        void send();

        bool wait();

        bool wait(Int32 timeout);

    private:

        HANDLE handle;
    };
}

#endif // NSF_OS_SIGNAL_WIN32_H

