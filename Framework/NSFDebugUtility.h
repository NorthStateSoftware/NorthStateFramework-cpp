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

#ifndef NSF_DEBUG_UTILITY_H
#define NSF_DEBUG_UTILITY_H

#include "NSFDataEvent.h"
#include "NSFEventHandler.h"
#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a debug utility class.
    /// </summary>
    class NSFDebugUtility : public NSFTaggedObject
    {
    public:

        /// <summary>
        /// Gets the primary framework debug utility.
        /// </summary>
        static NSFDebugUtility& getPrimaryDebugUtility() { static NSFDebugUtility debugUtility; return debugUtility; };

        /// <summary>
        /// Gets the maximum number of pending writes.
        /// </summary>
        /// <returns>
        /// The maximum number of pending writes.
        /// </returns>
        /// <remarks>
        /// Requests to write to console will be dropped if the maximum number of pending writes is exceeded.
        /// </remarks>
        int getMaxPendingWrites();

        /// <summary>
        /// Sets the maximum number of pending writes.
        /// </summary>
        /// <param name="value">Maximum number of pending writes.</param>
        /// <remarks>
        /// Requests to write to console will be dropped if the maximum number of pending writes is exceeded.
        /// </remarks>
        void setMaxPendingWrites(int value);

        /// <summary>
        /// Writes text to the console from a low priority thread.
        /// </summary>
        /// <param name="text">Text string to write to console.</param>
        /// <returns>
        /// False if the maximum number of pending writes is exceeded, true otherwise.
        /// </returns>
        bool writeToConsole(const NSFString& text);

        /// <summary>
        /// Writes a line of text to the console from a low priority thread.
        /// </summary>
        /// <param name="text">Text string to write to console.</param>
        /// <returns>
        /// False if the maximum number of pending writes is exceeded, true otherwise.
        /// </returns>
        bool writeLineToConsole(const NSFString& text);

        /// <summary>
        /// Checks if all output has been written to the console.
        /// </summary>
        /// <returns>
        /// True if all output text has been written, false otherwise.
        /// </returns>
        bool allOutputTextWritten();

    private:

        NSFEventHandler eventHandler;
        NSFDataEvent<NSFString> writeToConsoleEvent;
        NSFDataEvent<NSFString> writeLineToConsoleEvent;

        NSFOSMutex* debugUtilityMutex;
        int pendingWriteCount;
        int maxPendingWrites;
        bool lastWriteOnSeparateLine;
    
        // Static member used to force lazy instantiation during program startup.
        // Do not use for any other purpose.
        static NSFDebugUtility& debugUtility;

        /// <summary>
        /// Creates a debug utility.
        /// </summary>
        NSFDebugUtility();

        /// <summary>
        /// Destroys a debug utility
        /// </summary>
        virtual ~NSFDebugUtility();

        /// <summary>
        /// Reaction that performs the work of writing to the console.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void performWriteToConsole(const NSFEventContext& context);

        /// <summary>
        /// Reaction that performs the work of writing a line to the console.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void performWriteLineToConsole(const NSFEventContext& context);
    };
}

#endif // NSF_DEBUG_UTILITY_H
