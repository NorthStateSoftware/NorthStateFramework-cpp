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

#ifndef NSF_REGION_H
#define NSF_REGION_H

#include "NSFDelegates.h"
#include "NSFEventHandler.h"
#include "NSFStateMachineTypes.h"
#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a composite state region for nested substates.
    /// </summary>
    /// <remarks>
    /// Multiple regions within a composite state indicate concurrent behaviors.
    /// </remarks>
    class NSFRegion : public NSFTaggedObject
    {
    public:

        friend class NSFCompositeState;
        friend class NSFDeepHistory;
        friend class NSFForkJoin;
        friend class NSFShallowHistory;
        friend class NSFState;

        /// <summary>
        /// Creates a region.
        /// </summary>
        /// <param name="name">The name of the region.</param>
        /// <param name="parentState">The parent state of the region.</param>
        NSFRegion(const NSFString& name, NSFCompositeState* parentState);

        /// <summary>
        /// Indicates if the region's active substate is the specified state.
        /// </summary>
        /// <param name="state">The state in question.</param>
        /// <returns>True if the region's active substate is the specified state, false otherwise.</returns>
        bool isInState(NSFState* state);

        /// <summary>
        /// Indicates if the region's active substate is the specified state.
        /// </summary>
        /// <param name="stateName">The name of the state in question.</param>
        /// <returns>True if the region's active substate is the specified state, false otherwise.</returns>
        bool isInState(const NSFString& stateName);

    protected:


    private:

        bool active;
        NSFState* activeSubstate;
        NSFState* historySubstate;
        NSFState* initialState;
        NSFCompositeState* parentState;
        std::list<NSFState*> substates;

        /// <summary>
        /// Gets the region's history substate.
        /// </summary>
        NSFState* getHistorySubstate() const { return historySubstate; }

        /// <summary>
        /// Gets the region's parent state.
        /// </summary>
        NSFState* getParentState();

        /// <summary>
        /// Adds a substate to the region's list of substates.
        /// </summary>
        /// <param name="substate">The substate to add.</param>
        void addSubstate(NSFState* substate);

        /// <summary>
        /// Enters the region.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        /// <param name="useHistory">Flag indicating whether or not to use the history state as entry point.</param>
        void enter(NSFStateMachineContext& context, bool useHistory);

        /// <summary>
        /// Exits the region.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void exit(NSFStateMachineContext& context);

        /// <summary>
        /// Checks if the region is active.
        /// </summary>
        bool isActive() { return active; }

        /// <summary>
        /// Processes an event.
        /// </summary>
        /// <param name="nsfEvent">The event to process</param>
        /// <returns>NSFEventHandled if the event is acted upon, otherwise NSFEventUnhandled.</returns>
        NSFEventStatus processEvent(NSFEvent* nsfEvent);

        /// <summary>
        /// Resets the region to its initial condition.
        /// </summary>
        void reset();

        /// <summary>
        /// Sets the region's active substate.
        /// </summary>
        /// <param name="substate">The new active substate.</param>
        void setActiveSubstate(NSFState* value);
    };
}

#endif // NSF_REGION_H
