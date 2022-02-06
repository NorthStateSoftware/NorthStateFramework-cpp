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

#ifndef NSF_TRANSITION_H
#define NSF_TRANSITION_H

#include "NSFState.h"
#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a state transition.
    /// </summary>
    /// <remarks>
    /// This class implements common behaviors for the concrete transition types:
    /// NSFInternalTransition, NSFLocalTransition, NSFExternalTransition.
    /// Transitions may specify event triggers, guards, and/or actions.
    /// </remarks>
    class NSFTransition : public NSFTaggedObject
    {
    public:

        friend class NSFChoiceState;
        friend class NSFForkJoin;
        friend class NSFState;
        friend class NSFStateMachine;

        /// <summary>
        /// Actions to be executed whenever the transition is taken.
        /// </summary>
        NSFVoidActions<NSFStateMachineContext> Actions;

        /// <summary>
        /// Guards to be evaluated to determine if the transition can be taken.
        /// </summary>
        NSFBoolGuards<NSFStateMachineContext> Guards;

        /// <summary>
        /// Gets the source of the transition.
        /// </summary>
        NSFState* getSource() const { return source; }

        /// <summary>
        /// Gets the target of the transition.
        /// </summary>
        NSFState* getTarget() const { return target; }

        /// <summary>
        /// Gets the list of trigger events for the transition.
        /// </summary>
        /// <remarks>
        /// Returns a copy of the internally used list.
        /// Use addTrigger to add a new trigger.
        /// </remarks>
        std::list<NSFEvent*> getTriggers() const { return triggers; }

        /// <summary>
        /// Adds a trigger to the list of trigger events.
        /// </summary>
        void addTrigger(NSFEvent* value);

    protected:

        /// <summary>
        /// Creates a transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>& action);

        /// <summary>
        /// Creates a transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>& action);

        /// <summary>
        /// Creates a transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>* action);

        /// <summary>
        /// Creates a transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>* action);

        /// <summary>
        /// Destroys a transition.
        /// </summary>
        virtual ~NSFTransition();

        /// <summary>
        /// Processes an event, evaluating if the event results in the transition firing.
        /// </summary>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// </remarks>
        NSFEventStatus processEvent(NSFEvent* nsfEvent);

        /// <summary>
        /// Fires the transition.
        /// </summary>
        /// <param name="context">The state machine context associated with the transition firing.</param>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// </remarks>
        virtual void fireTransition(NSFStateMachineContext& context) = 0;

        /// <summary>
        /// Sets the source of the transition.
        /// </summary>
        virtual void setSource(NSFState* source);

        /// <summary>
        /// Sets the target of the transition.
        /// </summary>
        void setTarget(NSFState* target);

    private:

        NSFState* source;
        NSFState* target;
        std::list<NSFEvent*> triggers;

        /// <summary>
        /// Performs common contruction behaviors.
        /// </summary>
        void construct(NSFEvent* trigger);

        /// <summary>
        /// Handles an exception that occured while performing a transition action.
        /// </summary>
        /// <param name="context">The exception context.</param>
        void handleGuardException(const NSFExceptionContext& context);

        /// <summary>
        /// Handles an exception that occured while evaluating a transition guard.
        /// </summary>
        /// <param name="context">The exception context.</param>
        void handleActionException(const NSFExceptionContext& context);
    };
}

#endif // NSF_TRANSITION_H
