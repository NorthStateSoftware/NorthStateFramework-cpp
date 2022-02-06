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

#ifndef NSF_ENVIRONMENT_H
#define NSF_ENVIRONMENT_H

#include <list>

#include "NSFEventHandler.h"
#include "NSFThread.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents information and methods applicable to the entire framework environment.
    /// </summary>
    class NSFEnvironment
    {
    public:

        friend class NSFThread;

        /// <summary>
        /// Gets a list of threads in the framework enviroment.
        /// </summary>
        std::list<NSFThread*> getThreads();

        /// <summary>
        /// Starts the framework environment.
        /// </summary>
        /// <remarks>
        /// This method only needs to be called if the switch NSF_AUTO_START
        /// is not defined in NSFCustomConfig.h.  It should be called early
        /// in the application startup process, as soon as threads can be
        /// created, and before other framework classes are used.
        /// </remarks>
        void start();

        /// <summary>
        /// Gets the framework environment.
        /// </summary>
        static NSFEnvironment& getEnvironment() { static NSFEnvironment environment; return environment; }

    private:

        NSFOSMutex* environmentMutex;
        std::list<NSFThread*> threadList;

        // Static member used to force lazy instantiation during program startup.
        // Do not use for any other purpose.  Use getEnvironment() instead.
        static NSFEnvironment& environment;
        // Static member used to auto-start threaded environment features during program startup.
        // Do not use for any other purpose.
        static bool autoStarted;

        /// <summary>
        /// Creates the framework environment.
        /// </summary>
        NSFEnvironment();

        /// <summary>
        /// Destroys the framework environment after terminating.
        /// </summary>
        ~NSFEnvironment();

        /// <summary>
        /// Starts the threaded environment features if NSF_AUTO_START is defined.
        /// </summary>
        /// <returns>True if auto-started, false otherwise.</returns>
        bool autoStart();

        /// <summary>
        /// Adds a thread to the framework environment's list of threads.
        /// </summary>
        void addThread(NSFThread* thread);

        /// <summary>
        /// Removes a thread from the framework environment's list of threads.
        /// </summary>
        void removeThread(NSFThread* thread);

        /// <summary>
        /// Terminates the framework environment.
        /// </summary>
        void terminate();
    };
}

#endif // NSF_ENVIRONMENT_H
