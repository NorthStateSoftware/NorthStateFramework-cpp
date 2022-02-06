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
