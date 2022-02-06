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

#ifndef NSF_EXTERNAL_TRANSITION_H
#define NSF_EXTERNAL_TRANSITION_H

#include "NSFTransition.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an external state transition.
    /// </summary>
    /// <remarks>
    /// Transitions may specify an event trigger, guards, and/or a transition actions.
    /// External transitions force exiting of their source state.
    /// </remarks>
    class NSFExternalTransition : public NSFTransition
    {
    public:

        /// <summary>
        /// Creates an external transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFExternalTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>& action);

        /// <summary>
        /// Creates an external transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFExternalTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>& action);

        /// <summary>
        /// Creates an external transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFExternalTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>* action);

        /// <summary>
        /// Creates an external transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFExternalTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>* action);

    protected:

        virtual void setSource(NSFState* source);

        virtual void fireTransition(NSFStateMachineContext& context);
    };
}

#endif // NSF_EXTERNAL_TRANSITION_H
