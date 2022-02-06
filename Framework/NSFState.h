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

#ifndef NSF_STATE_H
#define NSF_STATE_H

#include "NSFDelegates.h"
#include "NSFEventHandler.h"
#include "NSFOSTypes.h"
#include "NSFStateMachineTypes.h"
#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a state machine state.
    /// </summary>
    /// <remarks>
    /// This class is used by the North State Framework as a base class for all state types.
    /// It is rarely used by external applications.
    /// They should use NSFCompositeState rather than NSFState, for greater flexibility and extensibility.
    /// </remarks>
    class NSFState : public NSFTaggedObject
    {
    public:

        friend class NSFChoiceState;
        friend class NSFCompositeState;
        friend class NSFDeepHistory;
        friend class NSFExternalTransition;
        friend class NSFForkJoin;
        friend class NSFInternalTransition;
        friend class NSFLocalTransition;
        friend class NSFRegion;
        friend class NSFShallowHistory;
        friend class NSFStateMachine;
        friend class NSFTransition;

        /// <summary>
        /// Creates a state.
        /// </summary>
        /// <param name="name">The name of the state.</param>
        /// <param name="parentRegion">The parent region of the state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the state.</param>
        NSFState(const NSFString& name, NSFRegion* parentRegion, const NSFVoidAction<NSFStateMachineContext>& entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction);

        /// <summary>
        /// Creates a state.
        /// </summary>
        /// <param name="name">The name of the state.</param>
        /// <param name="parentState">The parent state of the state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the state.</param>
        NSFState(const NSFString& name, NSFCompositeState* parentState, const NSFVoidAction<NSFStateMachineContext>& entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction);

        /// <summary>
        /// Creates a state.
        /// </summary>
        /// <param name="name">The name of the state.</param>
        /// <param name="parentRegion">The parent region of the state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the state.</param>
        NSFState(const NSFString& name, NSFRegion* parentRegion, NSFVoidAction<NSFStateMachineContext>* entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction);

        /// <summary>
        /// Creates a state.
        /// </summary>
        /// <param name="name">The name of the state.</param>
        /// <param name="parentState">The parent state of the state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the state.</param>
        NSFState(const NSFString& name, NSFCompositeState* parentState, NSFVoidAction<NSFStateMachineContext>* entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction);

        /// <summary>
        /// Creates a state.
        /// </summary>
        /// <param name="name">The name of the state.</param>
        /// <param name="parentRegion">The parent region of the state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the state.</param>
        NSFState(const NSFString& name, NSFRegion* parentRegion, const NSFVoidAction<NSFStateMachineContext>& entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction);

        /// <summary>
        /// Creates a state.
        /// </summary>
        /// <param name="name">The name of the state.</param>
        /// <param name="parentState">The parent state of the state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the state.</param>
        NSFState(const NSFString& name, NSFCompositeState* parentState, const NSFVoidAction<NSFStateMachineContext>& entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction);

        /// <summary>
        /// Creates a state.
        /// </summary>
        /// <param name="name">The name of the state.</param>
        /// <param name="parentRegion">The parent region of the state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the state.</param>
        NSFState(const NSFString& name, NSFRegion* parentRegion, NSFVoidAction<NSFStateMachineContext>* entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction);

        /// <summary>
        /// Creates a state.
        /// </summary>
        /// <param name="name">The name of the state.</param>
        /// <param name="parentState">The parent state of the state.</param>
        /// <param name="entryAction">The actions to be performed upon entry to the state.</param>
        /// <param name="exitAction">The actions to be performed upon exit of the state.</param>
        NSFState(const NSFString& name, NSFCompositeState* parentState, NSFVoidAction<NSFStateMachineContext>* entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction);

        /// <summary>
        /// Destroys a state.
        /// </summary>
        ~NSFState();

        /// <summary>
        /// Actions to be executed whenever the state is entered.
        /// </summary>
        NSFVoidActions<NSFStateMachineContext> EntryActions;

        /// <summary>
        /// Actions to be executed whenever the state is exited.
        /// </summary>
        NSFVoidActions<NSFStateMachineContext> ExitActions;

        /// <summary>
        /// Gets the flag indicating if trace logging is enabled or disabled for the state.
        /// </summary>
        /// <remarks>
        /// If this flag and the state machine's logging enabled flag are set true,
        /// then entry into the state is logged in the trace log.
        /// </remarks>
        bool getLogEntry() const { return logEntry; }

        /// <summary>
        /// Sets the flag indicating if trace logging is enabled or disabled for the state.
        /// </summary>
        /// <param name="value">The value for the flag.</param>
        /// <remarks>
        /// If this flag and the state machine's logging enabled flag are set true,
        /// then entry into the state is logged in the trace log.
        /// </remarks>
        void setLogEntry(bool value) { logEntry = value; }

        /// <summary>
        /// Gets the top state machine encompassing the state.
        /// </summary>
        /// <returns>The top state machine.</returns>
        virtual NSFStateMachine* getTopStateMachine();

        /// <summary>
        /// Indicates if the state is currently active.
        /// </summary>
        /// <returns>True if the state is active, otherwise false.</returns>
        /// <remarks>
        /// When this method returns true, the state machine is said to be "in" this state.
        /// </remarks>
        bool isActive();

        /// <summary>
        /// Indicates if the specified state is active, i.e. is "in" the specified state.
        /// </summary>
        /// <param name="state">The state in question.</param>
        /// <returns>True if the specified state is active, otherwise false.</returns>
        virtual bool isInState(NSFState* state);

        /// <summary>
        /// Indicates if the specified state is active, i.e. is "in" the specified state.
        /// </summary>
        /// <param name="stateName">The name of the state in question.</param>
        /// <returns>True if the specified state is active, otherwise false.</returns>
        /// <remarks>
        /// State names should be unique.
        /// </remarks>
        virtual bool isInState(const NSFString& stateName);

    protected:

        /// <summary>
        /// Gets the null state.
        /// </summary>
        /// <returns>The null state.</returns>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// </remarks>
        static NSFState* getNullState() { static NSFState nullState("NullState", (NSFRegion*) NULL, NULL, NULL); return &nullState; }

        /// <summary>
        /// Gets the parent region.
        /// </summary>
        /// <returns>The parent region.</returns>
        NSFRegion* getParentRegion();

        /// <summary>
        /// Gets the parent state.
        /// </summary>
        /// <returns>The parent state, or null if the state is not a substate.</returns>
        virtual NSFState* getParentState();

        /// <summary>
        /// Gets the parent state machine.
        /// </summary>
        /// <returns>The parent state machine, or null if no parent state machine.</returns>
        NSFStateMachine* getParentStateMachine();

        /// <summary>
        /// Enters the state.
        /// </summary>
        /// <param name="context">The event arguments pertaining to the transition into the state.</param>
        /// <param name="useHistory">Used by history states to reconstitute state history.</param>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// </remarks>
        virtual void enter(NSFStateMachineContext& context, bool useHistory);

        /// <summary>
        /// Exits the state.
        /// </summary>
        /// <param name="context">The event arguments pertaining to the transition out of the state.</param>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// </remarks>
        virtual void exit(NSFStateMachineContext& context);

        /// <summary>
        /// Processes an event.
        /// </summary>
        /// <param name="nsfEvent">The event to process.</param>
        /// <returns>NSFEventStatus.EventHandled if the event is handled, otherwise NSFEventStatus.EventUnhandled.</returns>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// </remarks>
        virtual NSFEventStatus processEvent(NSFEvent* nsfEvent);

        /// <summary>
        /// Resets the state to its initial configuration.
        /// </summary>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// </remarks>
        virtual void reset();

    private:

        bool active;
        bool logEntry;

        NSFRegion* parentRegion;

        std::list<NSFTransition*> incomingTransitions;
        std::list<NSFTransition*> outgoingTransitions;

        // Static member used to force lazy instantiation during program startup.
        // Do not use for any other purpose.  Use getNullState() instead.
        static NSFState* nullState;

        /// <summary>
        /// Performs common construction behaviors.
        /// </summary>
        void construct();

        /// <summary>
        /// Adds an incoming transition.
        /// </summary>
        /// <param name="transition">The incoming transition to add.</param>
        /// <remarks>
        /// This method is called from the base transition class constructor.
        /// </remarks>
        void addIncomingTransition(NSFTransition* transition);

        /// <summary>
        /// Adds an outgoing transition.
        /// </summary>
        /// <param name="transition">The outgoing transition to add.</param>
        /// <remarks>
        /// This method is called from the internal transition class constructor.
        /// </remarks>
        void addOutgoingTransition(NSFInternalTransition* transition);

        /// <summary>
        /// Adds an outgoing transition.
        /// </summary>
        /// <param name="transition">The outgoing transition to add.</param>
        /// <remarks>
        /// This method is called from the local transition class constructor.
        /// </remarks>
        void addOutgoingTransition(NSFLocalTransition* transition);

        /// <summary>
        /// Adds an outgoing transition.
        /// </summary>
        /// <param name="transition">The outgoing transition to add.</param>
        /// <remarks>
        /// This method is called from the external transition class constructor.
        /// </remarks>
        void addOutgoingTransition(NSFExternalTransition* transition);

        /// <summary>
        /// Handles exceptions caught while executing entry actions.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void handleEntryActionException(const NSFExceptionContext& context);

        /// <summary>
        /// Handles exceptions caught while executing exit actions.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void handleExitActionException(const NSFExceptionContext& context);

        /// <summary>
        /// Indicates if this state is a parent of the specified substate.
        /// </summary>
        /// <param name="substate">The substate in question.</param>
        /// <returns>True if this state is a parent, false otherwise.</returns>
        bool isParent(NSFState* substate);

        /// <summary>
        /// Removes an incoming transition.
        /// </summary>
        /// <param name="transition">The incoming transition to remove.</param>
        /// <remarks>
        /// It is called when re-routing a transition.
        /// </remarks>
        void removeIncomingTransition(NSFTransition* transition);

        /// <summary>
        /// Removes an outgoing transition.
        /// </summary>
        /// <param name="transition">The outgoing transition to remove.</param>
        /// <remarks>
        /// It is called when re-routing a transition.
        /// </remarks>
        void removeOutgoingTransition(NSFTransition* transition);
    };
}

#endif // NSF_STATE_H
