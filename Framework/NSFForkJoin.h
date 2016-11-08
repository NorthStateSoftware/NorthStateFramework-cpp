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

#ifndef NSF_FORK_JOIN_H
#define NSF_FORK_JOIN_H

#include "NSFState.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a fork-join state.
    /// </summary>
    /// <remarks>
    /// Fork-join states are used to provide synchronization and branching across multiple regions.
    /// </remarks>
    class NSFForkJoin : public NSFState
    {
    public:

        /// <summary>
        /// Creates a fork-join state.
        /// </summary>
        /// <param name="name">The name of the fork-join state.</param>
        /// <param name="parentState">The parent state of the fork-join state.</param>
        NSFForkJoin(const NSFString& name, NSFCompositeState* parentState);

        /// <summary>
        /// Indicates if the fork-join is the active substate within the specified region.
        /// </summary>
        /// <param name="region">The region in question.</param>
        /// <returns>True if the fork-join is the active substate, otherwise false.</returns>
        bool isActive(NSFRegion* region);

    protected:

        virtual NSFState* getParentState();

        virtual void enter(NSFStateMachineContext& context, bool useHistory);

        virtual void exit(NSFStateMachineContext& context);

        virtual NSFEventStatus processEvent(NSFEvent* nsfEvent);

        virtual void reset();

    private:

        std::list<NSFTransition*> completedTransitions;
        NSFCompositeState* parentState;
    };
}

#endif // NSF_FORK_JOIN_H
