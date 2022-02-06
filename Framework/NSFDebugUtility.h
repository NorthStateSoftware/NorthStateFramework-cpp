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
