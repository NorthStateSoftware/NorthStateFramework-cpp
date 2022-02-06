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

#include "WorkHardPlayHard.h"

#include <iostream>

using namespace WorkHardPlayHardExample;

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

    WorkHardPlayHard workHardPlayHardExample("WorkHardPlayHardExample");
    workHardPlayHardExample.startStateMachine();

    NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("WorkHardPlayHardExample - Review trace file to see results."));

    // Illustrate transitions and history
    // Work for a while, then play for a while, then take a break, then back to playing via history, then back to work
    NSFOSThread::sleep(100);
    workHardPlayHardExample.milestoneMet();
    NSFOSThread::sleep(100);
    workHardPlayHardExample.takeBreak();
    NSFOSThread::sleep(100);
    workHardPlayHardExample.breakOver();
    NSFOSThread::sleep(100);
    workHardPlayHardExample.backToWork();
    NSFOSThread::sleep(100);

    // Save trace log
    NSFTraceLog::getPrimaryTraceLog().saveLog("WorkHardPlayHardExampleTrace.xml");

    NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Press Enter key to continue"));
    cin.get();

    return 0;
}

