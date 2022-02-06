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

#include "NSFOSThread.h"
#include "NSFExceptionHandler.h"

namespace NorthStateFramework
{
    // Public

    NSFOSThread::~NSFOSThread()
    {
        delete action;
    }

    void NSFOSThread::executeAction()
    {
        try
        {
            (*action)(NSFContext(this));
        }
        catch(const std::exception& exception)
        {
            handleException(std::runtime_error(NSFString("Execution action exception: ") + exception.what()));
        }
        catch(...)
        {
            handleException(std::runtime_error(NSFString("Execution action exception: unknown exception")));
        }
    }

    // Protected

    NSFOSThread::NSFOSThread(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
        : NSFTaggedObject(name), action(executionAction.copy())
    {
    }

    void NSFOSThread::handleException(const std::exception& exception)
    {
        std::runtime_error newException(getName() + " OS thread exception: " + exception.what());
        NSFExceptionHandler::getExceptionHandler().handleException(NSFExceptionContext(this, newException));
    }
}
