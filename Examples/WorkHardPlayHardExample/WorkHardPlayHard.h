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

#ifndef WORK_HARD_PLAY_HARD_H
#define WORK_HARD_PLAY_HARD_H

#include "NorthStateFramework.h"

namespace WorkHardPlayHardExample
{
    class WorkHardPlayHard : public NSFStateMachine
    {
    public:
        WorkHardPlayHard(const NSFString& name);

        ~WorkHardPlayHard();

        // Public interface
        void milestoneMet();
        void takeBreak();
        void breakOver();
        void backToWork();

    protected:

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        NSFEvent breakEvent;
        NSFEvent breakOverEvent;
        NSFEvent milestoneMetEvent;
        NSFEvent backToWorkEvent;

        // Regions and states, from outer to inner
        NSFInitialState workHardPlayHardInitialState;
        NSFCompositeState takeABreakState;
        NSFCompositeState breakOverState;
        NSFInitialState breakOverInitialState;
        NSFDeepHistory breakOverHistoryState;
        NSFCompositeState workHardState;
        NSFCompositeState playHardState;

        // Transitions, ordered internal, local, external
        NSFExternalTransition workHardPlayHardInitialToBreakOverTransition;
        NSFExternalTransition takeABreakToBreakOverTransition;
        NSFExternalTransition breakOverToTakeABreakTransition;
        NSFExternalTransition breakOverInitialToBreakOverHistoryTransition;
        NSFExternalTransition breakOverHistoryToWorkHardTransition;
        NSFExternalTransition workHardToPlayHardTransition;
        NSFExternalTransition playHardToWorkHardTransition;
    };
}

#endif // WORK_HARD_PLAY_HARD_H
