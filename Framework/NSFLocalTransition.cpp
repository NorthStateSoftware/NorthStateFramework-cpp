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
