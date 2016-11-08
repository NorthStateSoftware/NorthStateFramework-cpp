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
