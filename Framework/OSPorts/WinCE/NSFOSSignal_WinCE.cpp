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

#if defined NSF_OS_WINCE

#include "NSFOSSignal_WinCE.h"

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSSignal* NSFOSSignal::create(const NSFString& name)
    {
        return new NSFOSSignal_WinCE(name);
    }

    // Concrete class definitions

    // Public

    NSFOSSignal_WinCE::NSFOSSignal_WinCE(const NSFString& name)
        : NSFOSSignal(name), handle(NULL)
    {
        if ((handle = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
        {
            throw std::runtime_error(getName() + " unable to create signal");
        }
    }

    NSFOSSignal_WinCE::~NSFOSSignal_WinCE()
    {
        if (handle != NULL)
        {
            CloseHandle(handle);
        }
    }

    void NSFOSSignal_WinCE::clear()
    {
        wait(0);
    }

    void NSFOSSignal_WinCE::send()
    {
        SetEvent(handle);
    }

    bool NSFOSSignal_WinCE::wait()
    {
        return wait(INFINITE);
    }

    bool NSFOSSignal_WinCE::wait(Int32 timeout)
    {
        if (WaitForSingleObject(handle, timeout) != WAIT_OBJECT_0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

#endif // NSF_OS_WINCE

