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

#include "NSFCustomConfig.h"

#if defined NSF_OS_WIN32

#include "NSFOSMutex_Win32.h"

#include <stdexcept>

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSMutex* NSFOSMutex::create()
    {
        return new NSFOSMutex_Win32();
    }

    // Concrete class definitions

    // Public

    NSFOSMutex_Win32::NSFOSMutex_Win32()
#if defined _DEBUG
        : lockCounter(0)
#endif  // _DEBUG
    {
        InitializeCriticalSection(&criticalSection);
    }

    NSFOSMutex_Win32::~NSFOSMutex_Win32()
    {
        DeleteCriticalSection(&criticalSection);
    }

    void NSFOSMutex_Win32::lock()
    {
        EnterCriticalSection(&criticalSection);

#if defined _DEBUG
        if (++lockCounter > 1)
        {
            // Not all operating systems support recursive mutex locks, 
            // this performs a runtime check to ensure that those do not happen.
            throw std::runtime_error("Recursive Lock Detected");
        }
#endif  // _DEBUG
    }

    void NSFOSMutex_Win32::unlock()
    {
#if defined _DEBUG
        --lockCounter;
#endif  // _DEBUG

        LeaveCriticalSection(&criticalSection);
    }
}

#endif // NSF_OS_WIN32
