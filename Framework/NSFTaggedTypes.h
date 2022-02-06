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

#ifndef NSF_TAGGED_TYPES_H
#define NSF_TAGGED_TYPES_H

#include "NSFCoreTypes.h"
#include "NSFOSMutex.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an interface for objects containing a name.
    /// </summary>
    class INSFNamedObject
    {
    public:

        /// <summary>
        /// Destroys the object.
        /// </summary>
        virtual ~INSFNamedObject();

        /// <summary>
        /// Gets the name of the object.
        /// </summary>
        virtual const NSFString& getName() const = 0;

        /// <summary>
        /// Sets the name of the object.
        /// </summary>
        virtual void setName(const NSFString& value) = 0;
    };

    /// <summary>
    /// Represents an object containing a uniquely numbered id.
    /// </summary>
    /// <remarks>
    /// There are 2^64 unique ids available.
    /// </remarks>
    class NSFUniquelyNumberedObject
    {
    public:

        /// <summary>
        /// Creates a uniquely numbered object.
        /// </summary>
        NSFUniquelyNumberedObject();

        /// <summary>
        /// Destroys a uniquely numbered object.
        /// </summary>
        virtual ~NSFUniquelyNumberedObject();

        /// <summary>
        /// Gets the unique id of the object.
        /// </summary>
        NSFId getUniqueId() const { return uniqueId; }

        /// <summary>
        /// Checks if this object is the same as another object.
        /// </summary>
        /// <param name="other">The other object to compare against.</param>
        /// <returns>True if the objects have the same unique id, false otherwise.</returns>
        bool isSameObject(NSFUniquelyNumberedObject* other);

        /// <summary>
        /// Gets the next unique id.
        /// </summary>
        /// <remarks>
        /// There are 2^64 unique ids available.
        /// </remarks>
        static NSFId getNextUniqueId();

    private:

        NSFId uniqueId;

        static NSFOSMutex* getUniquelyNumberedObjectMutex();
    };

    /// <summary>
    /// Represents an object containing unique id and name
    /// </summary>
    class NSFTaggedObject : public NSFUniquelyNumberedObject, public virtual INSFNamedObject
    {
    public:

        /// <summary>
        /// Creates an object with a unique id and a name.
        /// </summary>
        /// <param name="name">The user defined name for the object.</param>
        NSFTaggedObject(const NSFString& name);

        /// <summary>
        /// Destroys a tagged object.
        /// </summary>
        virtual ~NSFTaggedObject();

        virtual const NSFString& getName() const { return name; }

        virtual void setName(const NSFString& value) { name = value.c_str(); }

    private:

        NSFString name;
    };
}

#endif // NSF_TAGGED_TYPES_H
