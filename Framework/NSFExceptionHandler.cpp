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

#include "NSFExceptionHandler.h"

#include "NSFTraceLog.h"

namespace NorthStateFramework
{
    // Public 

    void NSFExceptionHandler::handleException(const NSFExceptionContext& context)
    {
        try
        {
            NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::ExceptionTag(), NSFTraceTags::MessageTag(), context.getException().what());

            if (saveTraceOnException)
            {
                NSFTraceLog::getPrimaryTraceLog().saveLog(traceFileName);
            }
        }
        catch(...)
        {
            // Nothing to do
        }

        ExceptionActions.execute(context);
    }

    // Private

    NSFExceptionHandler& NSFExceptionHandler::exceptionHandler(getExceptionHandler());

    NSFExceptionHandler::NSFExceptionHandler()
        :  saveTraceOnException(true), traceFileName("ExceptionTraceFile.xml")
    {
    }

    NSFExceptionHandler::~NSFExceptionHandler()
    {
    }
}
