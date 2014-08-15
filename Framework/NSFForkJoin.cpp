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

#include "NSFForkJoin.h"

#include "NSFCompositeState.h"
#include "NSFRegion.h"
#include "NSFForkJoinTransition.h"

namespace NorthStateFramework
{
    // Public 

    NSFForkJoin::NSFForkJoin(const NSFString& name, NSFCompositeState* parentState)
        : NSFState(name, (NSFRegion*)NULL, NULL, NULL), parentState(parentState)
    {
    }

    bool NSFForkJoin::isActive(NSFRegion* region)
    {
        return region->isInState(this);
    }

    // Protected

    NSFState* NSFForkJoin::getParentState()
    {
        return parentState;
    }

    void NSFForkJoin::enter(NSFStateMachineContext& context, bool useHistory)
    {
        // Additional behavior

        // Enter parent state, if necessary
        if (!parentState->isActive())
        {
            parentState->enter(context, false);
        }

        // Add transition to list of completed transitions
        completedTransitions.push_back(context.getTransition());

        // Set associated region's active substate to this

        // Simple case of transition source is a state
        if (context.getTransition()->getSource()->getParentRegion() != NULL)
        {
            context.getTransition()->getSource()->getParentRegion()->setActiveSubstate(this);
        }
        else // A region can also be specified as part of a fork-join to fork-join transition
        {
            NSFForkJoinTransition* forkJoinTransition = dynamic_cast<NSFForkJoinTransition*>(context.getTransition());
            if ((forkJoinTransition != NULL) && (forkJoinTransition->getForkJoinRegion() != NULL))
            {
                forkJoinTransition->getForkJoinRegion()->setActiveSubstate(this);
            }
        }

        // Base class behavior
        NSFState::enter(context, useHistory);
    }

    void NSFForkJoin::exit(NSFStateMachineContext& context)
    {
        // Additional behavior

        if (!active)
        {
            return;
        }

        // Set all associated regions' active substates to null state
        std::list<NSFTransition*>::iterator incomingTransitionIterator;
        for (incomingTransitionIterator = incomingTransitions.begin(); incomingTransitionIterator != incomingTransitions.end(); ++incomingTransitionIterator)
        {
            // Simple case of transition source is a state
            if ((*incomingTransitionIterator)->getSource()->getParentRegion() != NULL)
            {
                (*incomingTransitionIterator)->getSource()->getParentRegion()->setActiveSubstate(NSFState::getNullState());
            }
            else // A region can also be specified as part of a fork-join to fork-join transition
            {
                NSFForkJoinTransition* forkJoinTransition = dynamic_cast<NSFForkJoinTransition*>(*incomingTransitionIterator);
                if ((forkJoinTransition != NULL) && (forkJoinTransition->getForkJoinRegion() != NULL))
                {
                    forkJoinTransition->getForkJoinRegion()->setActiveSubstate(NSFState::getNullState());
                }
            }
        }

        // Remove all completed transitions
        completedTransitions.clear();

        // Base class behavior
        NSFState::exit(context);
    }

    NSFEventStatus NSFForkJoin::processEvent(NSFEvent* nsfEvent)
    {
        // Check if all incoming transitions are satisfied
        bool incomingTransitionsSatisfied = true;

        std::list<NSFTransition*>::iterator incomingTransitionIterator;
        for (incomingTransitionIterator = incomingTransitions.begin(); incomingTransitionIterator != incomingTransitions.end(); ++incomingTransitionIterator)
        {
            // Look for specific incoming transition in list of completed transitions
            // If find fails, it returns completedTransitions.end()
            if (std::find(completedTransitions.begin(), completedTransitions.end(), *incomingTransitionIterator) == completedTransitions.end())
            {
                incomingTransitionsSatisfied = false;
                break;
            }
        }

        if (incomingTransitionsSatisfied)
        {
            // Take all outgoing transitions
            std::list<NSFTransition*>::iterator outgoingTransitionIterator;
            for (outgoingTransitionIterator = outgoingTransitions.begin(); outgoingTransitionIterator != outgoingTransitions.end(); ++outgoingTransitionIterator)
            {
                // All outgoing transitions must be null transitions by UML2.x standard
                (*outgoingTransitionIterator)->processEvent(nsfEvent);
            }

            return NSFEventHandled;
        }

        return NSFEventUnhandled;
    }

    void NSFForkJoin::reset()
    {
        // Base class behavior
        NSFState::reset();

        // Additional behavior

        // Remove all completed transition sources
        completedTransitions.clear();
    }
}
