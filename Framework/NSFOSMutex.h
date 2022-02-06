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
