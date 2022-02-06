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

#include "NSFTransition.h"

#include "NSFEvent.h"
#include "NSFState.h"
#include "NSFStateMachine.h"

namespace NorthStateFramework
{
    // Public

    void NSFTransition::addTrigger(NSFEvent* trigger)
    {
        if (trigger == NULL)
        {
            return;
        }

        triggers.push_back(trigger);
    }

    // Protected

    NSFTransition::NSFTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>& action)
        : NSFTaggedObject(name), Actions(action), Guards(guard), source(source), target(target)
    {
        construct(trigger);
    }

    NSFTransition::NSFTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>& action)
        : NSFTaggedObject(name), Actions(action), Guards(guard), source(source), target(target)
    {
        construct(trigger);
    }

    NSFTransition::NSFTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>& guard, const NSFVoidAction<NSFStateMachineContext>* action)
        : NSFTaggedObject(name), Actions(action), Guards(guard), source(source), target(target)
    {
        construct(trigger);
    }

    NSFTransition::NSFTransition(const NSFString& name, NSFState* source, NSFState* target, NSFEvent* trigger, const NSFBoolGuard<NSFStateMachineContext>* guard, const NSFVoidAction<NSFStateMachineContext>* action)
        : NSFTaggedObject(name), Actions(action), Guards(guard), source(source), target(target)
    {
        construct(trigger);
    }

    NSFTransition::~NSFTransition()
    {
    }

    NSFEventStatus NSFTransition::processEvent(NSFEvent* nsfEvent)
    {
        bool transitionTriggered = false;

        if (triggers.empty())
        {
            transitionTriggered = true;
        }
        else
        {
            std::list<NSFEvent*>::iterator triggerIterator;
            for (triggerIterator = triggers.begin(); triggerIterator != triggers.end(); ++triggerIterator)
            {
                if ((*triggerIterator)->getId() == nsfEvent->getId())
                {
                    transitionTriggered = true;
                    break;
                }
            }
        }

        if (!transitionTriggered)
        {
            return NSFEventUnhandled;
        }

        NSFStateMachineContext context(getSource()->getTopStateMachine(), NULL, NULL, this, nsfEvent);

        if (!Guards.execute(context))
        {
            return NSFEventUnhandled;
        }

        fireTransition(context);

        return NSFEventHandled;
    }

    void NSFTransition::setSource(NSFState* value)
    {
        source = value;
        // Outgoing transitions must be added by concrete classes
    }

    void NSFTransition::setTarget(NSFState* value)
    {
        target = value;
        target->addIncomingTransition(this);
    }

    // Private

    void NSFTransition::construct(NSFEvent* trigger)
    {
        addTrigger(trigger);

        // Validity check
        if ((source == target) && (triggers.empty()) && Guards.isEmpty())
        {
            throw std::runtime_error(getName() + " invalid self-transition with no trigger or guard");
        }

        target->addIncomingTransition(this);
        // Outgoing transitions must be added by concrete classes

        Guards.setExceptionAction(NSFAction(this, &NSFTransition::handleGuardException));
        Actions.setExceptionAction(NSFAction(this, &NSFTransition::handleActionException));
    }

    void NSFTransition::handleGuardException(const NSFExceptionContext& context)
    {
        source->getTopStateMachine()->handleException(std::runtime_error(getName() + " transition guard exception: " + context.getException().what()));
    }

    void NSFTransition::handleActionException(const NSFExceptionContext& context)
    {
        source->getTopStateMachine()->handleException(std::runtime_error(getName() + " transition action exception: " + context.getException().what()));
    }
}
