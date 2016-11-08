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
