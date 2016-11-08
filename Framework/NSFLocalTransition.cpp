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

#include "NSFLocalTransition.h"

#include "NSFEvent.h"
#include "NSFState.h"
#include "NSFStateMachine.h"

namespace NorthStateFramework
{
    // Public

    NSFLocalTransition::NSFLocalTransition(const NSFString& name, NSFCompositeState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>& action)
        : NSFTransition(name, source, target, trigger, guard, action), compositeSource(source)
    {
        construct();
    }

    NSFLocalTransition::NSFLocalTransition(const NSFString& name, NSFCompositeState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>& action)
        : NSFTransition(name, source, target, trigger, guard, action), compositeSource(source)
    {
        construct();
    }

    NSFLocalTransition::NSFLocalTransition(const NSFString& name, NSFCompositeState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>* action)
        : NSFTransition(name, source, target, trigger, guard, action), compositeSource(source)
    {
        construct();
    }

    NSFLocalTransition::NSFLocalTransition(const NSFString& name, NSFCompositeState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>* action)
        : NSFTransition(name, source, target, trigger, guard, action), compositeSource(source)
    {
        construct();
    }

    // Protected

    void NSFLocalTransition::fireTransition(NSFStateMachineContext& context)
    {
        compositeSource->exitRegions(context);

        // Reset context possibly changed by exiting states
        context.setEnteringState(NULL);
        context.setExitingState(NULL);

        Actions.execute(context);

        if (getSource() != getTarget())
        {
            getTarget()->enter(context, false);
        }
        else
        {
            compositeSource->enterRegions(context, false);
        }
    }

    void NSFLocalTransition::setSource(NSFState* source)
    {
        NSFTransition::setSource(source);
        getSource()->addOutgoingTransition(this);
    }

    // Private

    void NSFLocalTransition::construct()
    {
        // Target must be substate of source or the source itself
        if ((!getSource()->isParent(getTarget())) && (getSource() != getTarget()))
        {
            throw std::runtime_error(getName() + " invalid local transition, source is neither parent of nor equal to target");
        }

        getSource()->addOutgoingTransition(this);
    }
}
