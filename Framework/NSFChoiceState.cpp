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
