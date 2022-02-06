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
