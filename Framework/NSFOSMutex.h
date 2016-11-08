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

#ifndef NSF_OS_MUTEX_H
#define NSF_OS_MUTEX_H

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an operating system mutex that may be used to provide mutual exclusion for a shared resource.
    /// </summary>
    class NSFOSMutex
    {
    public:

        /// <summary>
        /// Destroys a mutex.
        /// </summary>
        virtual ~NSFOSMutex();

        /// <summary>
        /// Creates an operating system mutex.
        /// </summary>
        /// </remarks>
        /// This is the factory method for creating a mutex specific to the OS.
        /// A mutex is unlocked at creation.
        /// </remarks>
        static NSFOSMutex* create();

        /// <summary>
        /// Locks the mutex.
        /// </summary>
        /// </remarks>
        /// If the mutex is already locked when the calling thread calls lock,
        /// then the calling thread will block until the mutex is unlocked.
        /// </remarks>
        virtual void lock() = 0;

        /// <summary>
        /// Unlocks the mutex.
        /// </summary>
        virtual void unlock() = 0;

    protected:

        /// <summary>
        /// Creates an operating system mutex.
        /// </summary>
        /// </remarks>
        /// A mutex is unlocked at creation.
        /// </remarks>
        NSFOSMutex() {}
    };

    /// <summary>
    /// Represents an object the locks and unlocks a mutex on construction and destruction, respectively.
    /// </summary>
    /// </remarks>
    /// This class supports the macros LOCK / ENDLOCK which make using mutexes more convenient.
    /// It implements the “Resource Acquisition Is Initialization” idiom to guarantee exception safe mutex usage.
    /// The following example illustrates how to use the macros.
    /// LOCK(getSomeMutex())
    /// {
    ///   // Access some shared resource
    ///   … 
    /// }
    /// ENDLOCK;
    /// </remarks>
    class NSFGuardWithMutex
    {
    public:

        /// <summary>
        /// Creates the class supporting macros LOCK / ENDLOCK.
        /// </summary>
        NSFGuardWithMutex(NSFOSMutex* mutex)
            : mutex(mutex)
        {
            mutex->lock();
        }

        /// <summary>
        /// Creates the class supporting macros LOCK / ENDLOCK.
        /// </summary>
        NSFGuardWithMutex(NSFOSMutex& mutex)
            : mutex(&mutex)
        {
            mutex.lock();
        }

        /// <summary>
        /// Deletes the class supporting macros LOCK / ENDLOCK.
        /// </summary>
        ~NSFGuardWithMutex()
        {
            mutex->unlock();
        }

    private:

        NSFOSMutex* mutex;
    };
}

#define LOCK(mutex) { NSFGuardWithMutex guard(mutex);
#define ENDLOCK }


#endif // NSF_OS_MUTEX_H
