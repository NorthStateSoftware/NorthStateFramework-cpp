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

#include "NSFChoiceState.h"

#include "NSFTransition.h"

namespace NorthStateFramework
{
    // Public

    NSFChoiceState::NSFChoiceState(const NSFString& name, NSFRegion* parentRegion)
        : NSFState(name, parentRegion, NULL, NULL)
    {
    }

    NSFChoiceState::NSFChoiceState(const NSFString& name, NSFCompositeState* parentState)
        : NSFState(name, parentState, NULL, NULL)
    {
    }

    // Protected

    NSFEventStatus NSFChoiceState::processEvent(NSFEvent* nsfEvent)
    {
        NSFTransition* elseTransition = NULL;

        // Check transitions out of state
        std::list<NSFTransition*>::iterator transitionIterator;
        for (transitionIterator = outgoingTransitions.begin(); transitionIterator != outgoingTransitions.end(); ++transitionIterator)
        {
            if (!(*transitionIterator)->Guards.isEmpty())
            {
                if ((*transitionIterator)->processEvent(nsfEvent) == NSFEventHandled)
                {
                    return NSFEventHandled;
                }
            }
            else
            {
                if (elseTransition != NULL)
                {
                    // A choice is ill formed if it has more than one "else" transition
                    throw std::runtime_error(getName() + " invalid choice state with multiple else transitions");
                }

                elseTransition = (*transitionIterator);
            }
        }

        if ((elseTransition != NULL) && (elseTransition->processEvent(nsfEvent) == NSFEventHandled))
        {
            return NSFEventHandled;
        }

        // If no transition is taken, then the model is ill formed and an exception is thrown.
        throw std::runtime_error(getName() + " invalid model with no transition taken from a choice state");
    }
}
