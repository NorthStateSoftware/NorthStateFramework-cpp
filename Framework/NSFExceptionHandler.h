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
