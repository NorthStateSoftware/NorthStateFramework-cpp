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

#include <iostream>
#include "CommandProcessor.h"
#include "CommandProcessorObserver.h"

using namespace CommandProcessorExample;

void globalExceptionAction(const NSFExceptionContext& context)
{
    NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Exception caught: ") + context.getException().what());
}

int main()
{
    // If NSF_AUTO_START is defined in NSFCustomConfig.h (default behavior), then calling start is not necessary.
    // NSFEnvironment::getEnvironment().start();

    NSFExceptionHandler::getExceptionHandler().ExceptionActions += NSFAction(&globalExceptionAction);

    NSFTraceLog::getPrimaryTraceLog().setEnabled(true);

    CommandProcessor commandProcessor(NSFString("CommandProcessor"));
    CommandProcessorObserver commandProcessorObserver(commandProcessor);

    commandProcessor.startStateMachine();

    // Simple example of polling for state
    int i = 0;
    while (true)
    {
        NSFOSThread::sleep(500);

        if (commandProcessor.isInState(&commandProcessor.getWaitForCommandState()))
        {
            NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Press Enter key to inject a command"));
            cin.get();
            commandProcessor.addCommand(NSFString("TestCommand"));
        }
        else if (commandProcessor.isInState(&commandProcessor.getWaitForResponseState()))
        {
            // Only send two responses
            if (++i <= 2)
            {
                commandProcessor.addResponse(NSFString("TestResponse"));
            }	 
        }
        else if (commandProcessor.isInState(&commandProcessor.getErrorState()))
        {
            break;
        }
    }

    NSFTraceLog::getPrimaryTraceLog().saveLog("CommandProcessorExampleTrace.xml");

    NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Press Enter key to continue"));
    cin.get();

    return 0;
}

