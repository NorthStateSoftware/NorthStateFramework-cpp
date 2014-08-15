// Copyright 2004-2014, North State Software, LLC.  All rights reserved.

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
