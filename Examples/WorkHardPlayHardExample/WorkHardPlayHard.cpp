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

namespace WorkHardPlayHardExample
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    WorkHardPlayHard::WorkHardPlayHard(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        // Event constructors take the form (name, parent)
        breakEvent("Break", this),
        breakOverEvent("BreakOver", this),
        milestoneMetEvent("MilestoneMet", this),
        backToWorkEvent("BackToWork", this),

        // Regions and states, from outer to inner 
        // Initial state construtors take the form (name, parent)
        workHardPlayHardInitialState("WorkHardPlayHardInitial", this),
        // Composite state construtors take the form (name, parent, entry actions, exit actions)
        takeABreakState("TakeABreak", this, NULL, NULL),
        breakOverState("BreakOver", this, NULL, NULL),
        breakOverInitialState("BreakOverInitial", &breakOverState),
        breakOverHistoryState("BreakOverHistory", &breakOverState),
        workHardState("WorkHard", &breakOverState, NULL, NULL),
        playHardState("PlayHard", &breakOverState, NULL, NULL),

        // Transitions, ordered internal, local, external
        // External transition construtors take the form (name, source, target, trigger, guard, action)
        workHardPlayHardInitialToBreakOverTransition("WorkHardPlayHardInitialToBreakOver", &workHardPlayHardInitialState, &breakOverState, NULL, NULL, NULL),
        takeABreakToBreakOverTransition("TakeABreakToBreakOver", &takeABreakState, &breakOverState, &breakOverEvent, NULL, NULL),
        breakOverToTakeABreakTransition("BreakOverToTakeABreak", &breakOverState, &takeABreakState, &breakEvent, NULL, NULL),
        breakOverInitialToBreakOverHistoryTransition("BreakOverInitialToBreakOverHistory", &breakOverInitialState, &breakOverHistoryState, NULL, NULL, NULL),
        breakOverHistoryToWorkHardTransition("BreakOverHistoryToWorkHard", &breakOverHistoryState, &workHardState, NULL, NULL, NULL),
        workHardToPlayHardTransition("WorkHardToPlayHard", &workHardState, &playHardState, &milestoneMetEvent, NULL, NULL),
        playHardToWorkHardTransition("PlayHardToWorkHard", &playHardState, &workHardState, &backToWorkEvent, NULL, NULL)
    {
    }

    WorkHardPlayHard::~WorkHardPlayHard()
    {
        // It is good practice to stop event processing (terminate) in the 
        // destructor of a state machine or event handler to prevent 
        // entry/exit actions from being performed on a destructing object.
        terminate(true);
        delete getEventThread();
    }

    void WorkHardPlayHard::milestoneMet()
    {
        queueEvent(&milestoneMetEvent);
    }

    void WorkHardPlayHard::takeBreak()
    {
        queueEvent(&breakEvent);
    }

    void WorkHardPlayHard::breakOver()
    {
        queueEvent(&breakOverEvent);
    }

    void WorkHardPlayHard::backToWork()
    {
        queueEvent(&backToWorkEvent);
    }




}