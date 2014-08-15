// Copyright 2004-2014, North State Software, LLC.  All rights reserved.

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

#ifndef NSF_INTERNAL_TRANSITION_H
#define NSF_INTERNAL_TRANSITION_H

#include "NSFTransition.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an internal state transition.
    /// </summary>
    /// <remarks>
    /// Transitions may specify event triggers, guards, and/or actions.
    /// Internal transitions do not exit or enter any states.  They are simply reactions to events.
    /// </remarks>
    class NSFInternalTransition : public NSFTransition
    {
    public:

        /// <summary>
        /// Creates an internal transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="state">Transition state.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFInternalTransition(const NSFString& name, NSFState* state, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>& action);

        /// <summary>
        /// Creates an internal transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="state">Transition state.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFInternalTransition(const NSFString& name, NSFState* state, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>& action);

        /// <summary>
        /// Creates an internal transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="state">Transition state.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFInternalTransition(const NSFString& name, NSFState* state, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>* action);

        /// <summary>
        /// Creates an internal transition.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="state">Transition state.</param>
        /// <param name="trigger">Transition trigger.</param>
        /// <param name="guard">Transition guard.</param>
        /// <param name="action">Transition action.</param>
        NSFInternalTransition(const NSFString& name, NSFState* state, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>* action);

    protected:

        virtual void fireTransition(NSFStateMachineContext& context);

        virtual void setSource(NSFState* source);
    };
}

#endif // NSF_INTERNAL_TRANSITION_H
