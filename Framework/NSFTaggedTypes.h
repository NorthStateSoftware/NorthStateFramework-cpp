// Copyright 2004-2016, North State Software, LLC.  All rights reserved.

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
