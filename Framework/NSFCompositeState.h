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

#ifndef NSF_COMPOSITE_STATE_H
#define NSF_COMPOSITE_STATE_H

#include "NSFState.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a state that may contain one or more orthogonal regions.
    /// </summary>
    /// <remarks>
    /// NSFCompositeState is the most commonly used state type.
    /// It affords the ability to nest regions and substates,
    /// making it flexible for extension.  There is minimal performance
    /// penalty for this capability, so this type should be preferred
    /// over the basic NSFState.
    /// </remarks>
    class NSFCompositeState : public NSFState
    {
    public:

        friend class NSFForkJoin;
        friend class NSFLocalTransition;
        friend class NSFRegion;
        friend class NSFState;

        /// <summary>
        /// Creates a composite state.
        /// </summary>
        /// <param name="name">The name of the composite state.</param>
        /// <param name="parentRegion">The parent region of the composite state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the composite state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the composite state.</param>
        NSFCompositeState(const NSFString& name, NSFRegion* parentRegion, const NSFVoidAction<NSFStateMachineContext>& entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction);

        /// <summary>
        /// Creates a composite state.
        /// </summary>
        /// <param name="name">The name of the composite state.</param>
        /// <param name="parentState">The parent state of the composite state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the composite state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the composite state.</param>
        NSFCompositeState(const NSFString& name, NSFCompositeState* parentState, const NSFVoidAction<NSFStateMachineContext>& entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction);

        /// <summary>
        /// Creates a composite state.
        /// </summary>
        /// <param name="name">The name of the composite state.</param>
        /// <param name="parentRegion">The parent region of the composite state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the composite state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the composite state.</param>
        NSFCompositeState(const NSFString& name, NSFRegion* parentRegion, NSFVoidAction<NSFStateMachineContext>* entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction);

        /// <summary>
        /// Creates a composite state.
        /// </summary>
        /// <param name="name">The name of the composite state.</param>
        /// <param name="parentState">The parent state of the composite state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the composite state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the composite state.</param>
        NSFCompositeState(const NSFString& name, NSFCompositeState* parentState, NSFVoidAction<NSFStateMachineContext>* entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction);

        /// <summary>
        /// Creates a composite state.
        /// </summary>
        /// <param name="name">The name of the composite state.</param>
        /// <param name="parentRegion">The parent region of the composite state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the composite state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the composite state.</param>
        NSFCompositeState(const NSFString& name, NSFRegion* parentRegion, const NSFVoidAction<NSFStateMachineContext>& entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction);

        /// <summary>
        /// Creates a composite state.
        /// </summary>
        /// <param name="name">The name of the composite state.</param>
        /// <param name="parentState">The parent state of the composite state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the composite state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the composite state.</param>
        NSFCompositeState(const NSFString& name, NSFCompositeState* parentState, const NSFVoidAction<NSFStateMachineContext>& entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction);

        /// <summary>
        /// Creates a composite state.
        /// </summary>
        /// <param name="name">The name of the composite state.</param>
        /// <param name="parentRegion">The parent region of the composite state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the composite state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the composite state.</param>
        NSFCompositeState(const NSFString& name, NSFRegion* parentRegion, NSFVoidAction<NSFStateMachineContext>* entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction);

        /// <summary>
        /// Creates a composite state.
        /// </summary>
        /// <param name="name">The name of the composite state.</param>
        /// <param name="parentState">The parent state of the composite state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the composite state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the composite state.</param>
        NSFCompositeState(const NSFString& name, NSFCompositeState* parentState, NSFVoidAction<NSFStateMachineContext>* entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction);

        /// <summary>
        /// Destroys a composite state.
        /// </summary>
        virtual ~NSFCompositeState();

        virtual bool isInState(NSFState* state);

        virtual bool isInState(const NSFString& stateName);

    protected:

        virtual void enter(NSFStateMachineContext& context, bool useHistory);

        virtual void exit(NSFStateMachineContext& context);

        virtual NSFEventStatus processEvent(NSFEvent* nsfEvent);

        virtual void reset();

    private:

        NSFRegion* defaultRegion;
        std::list<NSFRegion*> regions;

        /// <summary>
        /// Adds a region to the composite state.
        /// </summary>
        /// <param name="region">The region to add.</param>
        void addRegion(NSFRegion* region);

        /// <summary>
        /// Enters all the regions of the composite state.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        /// <param name="useHistory">Flag indicating if regions should be entered using the history substate.</param>
        void enterRegions(NSFStateMachineContext& context, bool useHistory);

        /// <summary>
        /// Exits all the regions of the composite state.
        /// </summary>
        void exitRegions(NSFStateMachineContext& context);

        /// <summary>
        /// Gets the composite state's default region.
        /// </summary>
        NSFRegion* getDefaultRegion();
    };
}

#endif // NSF_COMPOSITE_STATE_H
