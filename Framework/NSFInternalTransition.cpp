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

#include "NSFInternalTransition.h"

#include "NSFEvent.h"
#include "NSFState.h"
#include "NSFStateMachine.h"

namespace NorthStateFramework
{
    // Public 

    NSFInternalTransition::NSFInternalTransition(const NSFString& name, NSFState* state, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>& action)
        : NSFTransition(name, state, state, trigger, guard, action)
    {
        getSource()->addOutgoingTransition(this);
    }

    NSFInternalTransition::NSFInternalTransition(const NSFString& name, NSFState* state, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>& action)
        : NSFTransition(name, state, state, trigger, guard, action)
    {
        getSource()->addOutgoingTransition(this);
    }

    NSFInternalTransition::NSFInternalTransition(const NSFString& name, NSFState* state, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>* action)
        : NSFTransition(name, state, state, trigger, guard, action)
    {
        getSource()->addOutgoingTransition(this);
    }

    NSFInternalTransition::NSFInternalTransition(const NSFString& name, NSFState* state, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>* action)
        : NSFTransition(name, state, state, trigger, guard, action)
    {
        getSource()->addOutgoingTransition(this);
    }

    // Protected

    void NSFInternalTransition::fireTransition(NSFStateMachineContext& context)
    {
        Actions.execute(context);
    }

    void NSFInternalTransition::setSource(NSFState* source)
    {
        NSFTransition::setSource(source);
        getSource()->addOutgoingTransition(this);
    }
}
