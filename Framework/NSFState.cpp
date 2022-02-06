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

#include "NSFState.h"

#include "NSFDelegates.h"
#include "NSFCompositeState.h"
#include "NSFExternalTransition.h"
#include "NSFInternalTransition.h"
#include "NSFLocalTransition.h"
#include "NSFRegion.h"
#include "NSFStateMachine.h"
#include "NSFTraceLog.h"
#include "NSFTransition.h"

namespace NorthStateFramework
{
    NSFState* NSFState::nullState = NSFState::getNullState();

    // Public

    NSFState::NSFState(const NSFString& name, NSFRegion* parentRegion, const NSFVoidAction<NSFStateMachineContext>& entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction)
        : NSFTaggedObject(name), EntryActions(entryAction), ExitActions(exitAction), active(false), logEntry(true), parentRegion(parentRegion)
    {
        construct();
    }

    NSFState::NSFState(const NSFString& name, NSFCompositeState* parentState, const NSFVoidAction<NSFStateMachineContext>& entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction)
        : NSFTaggedObject(name), EntryActions(entryAction), ExitActions(exitAction), active(false), logEntry(true), parentRegion(parentState->getDefaultRegion())
    {
        construct();
    }

    NSFState::NSFState(const NSFString& name, NSFRegion* parentRegion, NSFVoidAction<NSFStateMachineContext>* entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction)
        : NSFTaggedObject(name), EntryActions(entryAction), ExitActions(exitAction), active(false), logEntry(true), parentRegion(parentRegion)
    {
        construct();
    }

    NSFState::NSFState(const NSFString& name, NSFCompositeState* parentState, NSFVoidAction<NSFStateMachineContext>* entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction)
        : NSFTaggedObject(name), EntryActions(entryAction), ExitActions(exitAction), active(false), logEntry(true), parentRegion(parentState->getDefaultRegion())
    {
        construct();
    }

    NSFState::NSFState(const NSFString& name, NSFRegion* parentRegion, const NSFVoidAction<NSFStateMachineContext>& entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction)
        : NSFTaggedObject(name), EntryActions(entryAction), ExitActions(exitAction), active(false), logEntry(true), parentRegion(parentRegion)
    {
        construct();
    }

    NSFState::NSFState(const NSFString& name, NSFCompositeState* parentState, const NSFVoidAction<NSFStateMachineContext>& entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction)
        : NSFTaggedObject(name), EntryActions(entryAction), ExitActions(exitAction), active(false), logEntry(true), parentRegion(parentState->getDefaultRegion())
    {
        construct();
    }

    NSFState::NSFState(const NSFString& name, NSFRegion* parentRegion, NSFVoidAction<NSFStateMachineContext>* entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction)
        : NSFTaggedObject(name), EntryActions(entryAction), ExitActions(exitAction), active(false), logEntry(true), parentRegion(parentRegion)
    {
        construct();
    }

    NSFState::NSFState(const NSFString& name, NSFCompositeState* parentState, NSFVoidAction<NSFStateMachineContext>* entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction)
        : NSFTaggedObject(name), EntryActions(entryAction), ExitActions(exitAction), active(false), logEntry(true), parentRegion(parentState->getDefaultRegion())
    {
        construct();
    }

    NSFState::~NSFState()
    {
    }

    NSFStateMachine* NSFState::getTopStateMachine()
    {
        return getParentState()->getTopStateMachine();
    }

    bool NSFState::isActive()
    {
        return active;
    }

    bool NSFState::isInState(NSFState* state)
    {
        if (!active)
        {
            return false;
        }

        return (this == state);
    }

    bool NSFState::isInState(const NSFString& stateName)
    {
        if (!active)
        {
            return false;
        }

        return (getName() == stateName);
    }

    // Protected

    NSFRegion* NSFState::getParentRegion()
    {
        return parentRegion;
    }

    NSFState* NSFState::getParentState()
    {
        if (parentRegion != NULL)
        {
            return parentRegion->getParentState();
        }
        else
        {
            return NULL;
        }
    }

    NSFStateMachine* NSFState::getParentStateMachine()
    {
        NSFState* parentState = getParentState();

        if (parentState == NULL)
        {
            return NULL;
        }

        if (NSFStateMachine* parentStateMachine = dynamic_cast<NSFStateMachine*>(parentState))
        {
            return parentStateMachine;
        }
        else
        {
            return parentState->getParentStateMachine();
        }
    }

    void NSFState::enter(NSFStateMachineContext& context, bool)
    {
        active = true;

        if (parentRegion != NULL)
        {
            parentRegion->setActiveSubstate(this);

            if (!parentRegion->isActive())
            {
                parentRegion->enter(context, false);
            }
        }

        if (getTopStateMachine()->getLoggingEnabled() && getLogEntry())
        {
            NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::StateEnteredTag(),
                NSFTraceTags::StateMachineTag(), getTopStateMachine()->getName(),
                NSFTraceTags::StateTag(), getName());
        }

        // Update context to indicate entering this state
        context.setEnteringState(this);
        context.setExitingState(NULL);

        EntryActions.execute(context);

        NSFStateMachine* parentStateMachine = getParentStateMachine();
        if (parentStateMachine != NULL)
        {
            parentStateMachine->executeStateChangeActions(context);
        }
    }

    void NSFState::exit(NSFStateMachineContext& context)
    {
        active = false;

        // Update context to indicate exiting this state
        context.setExitingState(this);
        context.setEnteringState(NULL);

        ExitActions.execute(context);

        if (parentRegion != NULL)
        {
            parentRegion->setActiveSubstate(NSFState::getNullState());
        }
    }

    NSFEventStatus NSFState::processEvent(NSFEvent* nsfEvent)
    {
        std::list<NSFTransition*>::iterator transitionIterator;
        for (transitionIterator = outgoingTransitions.begin(); transitionIterator != outgoingTransitions.end(); ++transitionIterator)
        {
            if ((*transitionIterator)->processEvent(nsfEvent) == NSFEventHandled)
            {
                return NSFEventHandled;
            }
        }

        return NSFEventUnhandled;
    }

    void NSFState::reset()
    {
        active = false;
    }

    // Private

    void NSFState::construct()
    {
        if (parentRegion != NULL)
        {
            parentRegion->addSubstate(this);
        }

        EntryActions.setExceptionAction(NSFAction(this, &NSFState::handleEntryActionException));
        ExitActions.setExceptionAction(NSFAction(this, &NSFState::handleExitActionException));
    }

    void NSFState::addIncomingTransition(NSFTransition* transition)
    {
        incomingTransitions.push_back(transition);
    }

    void NSFState::addOutgoingTransition(NSFInternalTransition* transition)
    {
        // Insert before first transition that is not internal
        std::list<NSFTransition*>::iterator transitionIterator;
        for (transitionIterator = outgoingTransitions.begin(); transitionIterator != outgoingTransitions.end(); transitionIterator++)
        {
            if (dynamic_cast<NSFInternalTransition*>(*transitionIterator) == NULL)
            {
                outgoingTransitions.insert(transitionIterator, transition);
                return;
            }
        }

        outgoingTransitions.push_back(transition);
    }

    void NSFState::addOutgoingTransition(NSFLocalTransition* transition)
    {
        // Insert before first external transition
        std::list<NSFTransition*>::iterator transitionIterator;
        for (transitionIterator = outgoingTransitions.begin(); transitionIterator != outgoingTransitions.end(); transitionIterator++)
        {
            if (dynamic_cast<NSFExternalTransition*>(*transitionIterator) != NULL)
            {
                outgoingTransitions.insert(transitionIterator, transition);
                return;
            }
        }

        outgoingTransitions.push_back(transition);
    }

    void NSFState::addOutgoingTransition(NSFExternalTransition* transition)
    {
        outgoingTransitions.push_back(transition);
    }

    void NSFState::handleEntryActionException(const NSFExceptionContext& context)
    {
        getTopStateMachine()->handleException(std::runtime_error(getName() + " state entry action exception: " + context.getException().what()));
    }

    void NSFState::handleExitActionException(const NSFExceptionContext& context)
    {
        getTopStateMachine()->handleException(std::runtime_error(getName() + " state exit action exception: " + context.getException().what()));
    }

    bool NSFState::isParent(NSFState* substate)
    {
        NSFState* substateParent = substate->getParentState();

        while (substateParent != NULL)
        {
            if (substateParent == this)
            {
                return true;
            }
            else
            {
                substateParent = substateParent->getParentState();
            }
        }

        return false;
    }

    void NSFState::removeIncomingTransition(NSFTransition* transition)
    {
        incomingTransitions.remove(transition);
    }

    void NSFState::removeOutgoingTransition(NSFTransition* transition)
    {
        outgoingTransitions.remove(transition);
    }
}
