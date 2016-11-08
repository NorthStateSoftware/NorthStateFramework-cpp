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

#ifndef NSF_EXCEPTION_HANDLER_H
#define NSF_EXCEPTION_HANDLER_H

#include "NSFDelegates.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents the global exception handler through which clients can receive notification of runtime exceptions.
    /// </summary>
    /// <remarks>
    /// Register exception actions with this class to receive notification of any runtime exception caught by the North State Framework.
    /// </remarks>
    class NSFExceptionHandler
    {
    public:

        /// <summary>
        /// Actions to execute when an exception is thrown.
        /// </summary>
        NSFVoidActions<NSFExceptionContext> ExceptionActions;

        /// <summary>
        /// Gets the global exception handler.
        /// </summary>
        static NSFExceptionHandler& getExceptionHandler() { static NSFExceptionHandler exceptionHandler; return exceptionHandler; }

        /// <summary>
        /// Gets the flag indicating if a trace should be saved on exception.
        /// </summary>
        bool getSaveTraceOnException() { return saveTraceOnException; }

        /// <summary>
        /// Sets the flag indicating if a trace should be saved on exception.
        /// </summary>
        void setSaveTraceOnException(bool value) { saveTraceOnException = value; }

        /// <summary>
        /// Gets the name for the exception trace file.
        /// </summary>
        /// <remarks>
        /// This property specifies the relative or fully qualified file name to save the trace log into.
        /// </remarks>
        const NSFString& getTraceFileName() { return traceFileName; }

        /// <summary>
        /// Gets the name for the exception trace file.
        /// </summary>
        /// <remarks>
        /// This property specifies the relative or fully qualified file name to save the trace log into.
        /// </remarks>
        void setTraceFileName(const NSFString& value) { traceFileName = value.c_str(); }

        /// <summary>
        /// Global exception handling method.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        /// <remarks>
        /// All exceptions are ultimately routed to this method. It logs the exception to the trace log,
        /// saves the trace log if a file name has been set, and executes any exception actions.
        /// </remarks>
        void handleException(const NSFExceptionContext& context);

    private:

        bool saveTraceOnException;
        NSFString traceFileName;

        // Static member used to force lazy instantiation during program startup.
        // Do not use for any other purpose.  Use getExceptionHandler() instead.
        static NSFExceptionHandler& exceptionHandler;

        NSFExceptionHandler();

        ~NSFExceptionHandler();
    };
}

#endif // NSF_EXCEPTION_HANDLER_H
