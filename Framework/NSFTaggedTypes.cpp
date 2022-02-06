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

#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    // INSFNamedObject

    // Public

    INSFNamedObject::~INSFNamedObject()
    {
    }

    // NSFUniquelyNumberedObject

    // Public

    NSFUniquelyNumberedObject::~NSFUniquelyNumberedObject()
    {
    }

    NSFUniquelyNumberedObject::NSFUniquelyNumberedObject()
        : uniqueId(getNextUniqueId())
    {
    }

    bool NSFUniquelyNumberedObject::isSameObject(NSFUniquelyNumberedObject* other)
    {
        return (getUniqueId() == other->getUniqueId());
    }

    NSFId NSFUniquelyNumberedObject::getNextUniqueId()
    {
        static NSFId nextUniqueId = 0;
        LOCK(getUniquelyNumberedObjectMutex())
        {
            return ++nextUniqueId;
        }
        ENDLOCK;
    }

    // Private

    NSFOSMutex* NSFUniquelyNumberedObject::getUniquelyNumberedObjectMutex()
    {
        static NSFOSMutex* idMutex = NSFOSMutex::create();
        return idMutex;
    }

    // NSFTaggedObject

    // Public

    NSFTaggedObject::NSFTaggedObject(const NSFString& name)
        : name(name.c_str())
    {
    }

    NSFTaggedObject::~NSFTaggedObject()
    {
    }
}
