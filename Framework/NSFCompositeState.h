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
