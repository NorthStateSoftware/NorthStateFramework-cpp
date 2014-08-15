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

