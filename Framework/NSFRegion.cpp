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

#include "NSFRegion.h"

#include "NSFCompositeState.h"
#include "NSFInitialState.h"
#include "NSFStateMachine.h"

namespace NorthStateFramework
{
    // Public 

    NSFRegion::NSFRegion(const NSFString& name, NSFCompositeState* parentState)
        : NSFTaggedObject(name), active(false), activeSubstate(NSFState::getNullState()), historySubstate(NSFState::getNullState()), initialState(NSFState::getNullState()), parentState(parentState)
    {
        if (parentState == NULL)
        {
            throw std::runtime_error(getName() + " invalid region with null parent state");
        }

        parentState->addRegion(this);
    }

    bool NSFRegion::isInState(NSFState* state)
    {
        if (!active)
        {
            return false;
        }

        return activeSubstate->isInState(state);
    }

    bool NSFRegion::isInState(const NSFString& stateName)
    {
        if (!active)
        {
            return false;
        }

        return activeSubstate->isInState(stateName);
    }

    // Private

    NSFState* NSFRegion::getParentState()
    {
        return parentState;
    }

    void NSFRegion::addSubstate(NSFState* substate)
    {
        substates.push_back(substate);

        // Any single state in a region can be the initial state
        // If more than one state exists in a region, there must be one and only one NSFInitialState

        // First substate is automatically registered as initial state
        if (substates.size() == 1)
        {
            initialState = substate;
            return;
        }

        // Not first state

        // If initial state is not already an NSFInitialState
        if (dynamic_cast<NSFInitialState*>(initialState) == NULL)
        {
            if (dynamic_cast<NSFInitialState*>(substate) != NULL)
            {
                initialState = substate;
            }
            else // Not adding an NSFInitialState
            {
                // Reset initial state to null, enforcing requirement for an NSFInitialState
                initialState = NSFState::getNullState();
            }
        }
        else // Initial state already established
        {
            if (dynamic_cast<NSFInitialState*>(substate) != NULL)
            {
                throw std::runtime_error(getName() + " only one initial state allowed in a region");
            }
        }
    }

    void NSFRegion::enter(NSFStateMachineContext& context, bool useHistory)
    {
        active = true;

        if (!parentState->isActive())
        {
            parentState->enter(context, false);
        }

        if (useHistory)
        {
            if (historySubstate != NSFState::getNullState())
            {
                historySubstate->enter(context, useHistory);
            }
        }

        if (activeSubstate == NSFState::getNullState())
        {
            initialState->enter(context, false);
        }
    }

    void NSFRegion::exit(NSFStateMachineContext& context)
    {
        active = false;

        if (activeSubstate != NSFState::getNullState())
        {
            activeSubstate->exit(context);
        }
    }

    NSFEventStatus NSFRegion::processEvent(NSFEvent* nsfEvent)
    {
        return activeSubstate->processEvent(nsfEvent);
    }

    void NSFRegion::reset()
    {
        active = false;

        activeSubstate = NSFState::getNullState();
        historySubstate = NSFState::getNullState();

        std::list<NSFState*>::iterator substateIterator;
        for (substateIterator = substates.begin(); substateIterator != substates.end(); ++substateIterator)
        {
            (*substateIterator)->reset();
        }
    }

    void NSFRegion::setActiveSubstate(NSFState* substate)
    {
        // Set history substate to the current, active substate whenever the active substate is about to becomes the null state,
        // except that the history substate should not be set to the null state or the initial state.
        if ((substate == NSFState::getNullState()) && (activeSubstate != NSFState::getNullState()) && (activeSubstate != initialState))
        {
            historySubstate = activeSubstate;
        }

        activeSubstate = substate;
    }
}
