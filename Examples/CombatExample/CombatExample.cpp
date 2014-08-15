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

#include <iostream>
#include "Combat.h"

using namespace CombatExample;

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

    Combat combatExample("CombatExample");
    combatExample.startStateMachine();

    NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("CombatExample - Review trace file to see results."));

    // Illustrate transitions taken as distance to enemy changes
    combatExample.sendScoutTeam();
    NSFOSThread::sleep(100);
    combatExample.setDistanceToEnemy(combatExample.getNearDistance() - 1);
    combatExample.sendScoutTeam();
    NSFOSThread::sleep(100);
    combatExample.setDistanceToEnemy(combatExample.getInRangeDistance() - 1);
    combatExample.sendScoutTeam();
    NSFOSThread::sleep(100);

    // Save trace log
    NSFTraceLog::getPrimaryTraceLog().saveLog("CombatExampleTrace.xml");

    NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Press Enter key to continue"));

    cin.get();

    return 0;
}

