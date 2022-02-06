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

#if defined NSF_OS_ECOS

#include "NSFOSMutex_eCOS.h"
#include <stdexcept>

namespace NorthStateFramework
{
    // Base class definitions

    NSFOSMutex* NSFOSMutex::create()
    {
        return new NSFOSMutex_eCOS();
    }

    // Concrete class definitions

    // Public

    NSFOSMutex_eCOS::NSFOSMutex_eCOS()
        : NSFOSMutex()
    {
        cyg_mutex_init(&mutex);
    }

    NSFOSMutex_eCOS::~NSFOSMutex_eCOS()
    {
        cyg_mutex_destroy(&mutex);
    }

    void NSFOSMutex_eCOS::lock()
    {
        if (cyg_mutex_lock(&mutex) == false)
        {
            throw std::runtime_error("Mutex failed to lock");
        }
    }

    void NSFOSMutex_eCOS::unlock()
    {
        cyg_mutex_unlock(&mutex);
    }
}

#endif // NSF_OS_ECOS

