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

#include "NSFShallowHistory.h"

#include "NSFRegion.h"

namespace NorthStateFramework
{
    // Public
    NSFShallowHistory::NSFShallowHistory(const NSFString& name, NSFRegion* parentRegion)
        : NSFState(name, parentRegion, NULL, NULL)
    {
    }

    NSFShallowHistory::NSFShallowHistory(const NSFString& name, NSFCompositeState* parentState)
        : NSFState(name, parentState, NULL, NULL)
    {
    }

    // Protected

    void NSFShallowHistory::enter(NSFStateMachineContext& context, bool useHistory)
    {
        // Base class behavior
        NSFState::enter(context, false);

        // Additional behavior

        // Enter history substate, not using recursive history
        NSFState* historySubstate = parentRegion->getHistorySubstate();
        if (historySubstate != NSFState::getNullState())
        {
            historySubstate->enter(context, false);
        }
    }
}
