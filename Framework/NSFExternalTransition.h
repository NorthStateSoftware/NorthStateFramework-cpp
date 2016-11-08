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
