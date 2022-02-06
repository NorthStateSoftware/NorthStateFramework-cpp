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

#ifndef NSF_CHOICE_STATE_H
#define NSF_CHOICE_STATE_H

#include "NSFState.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a choice pseudo-state.
    /// </summary>
    /// <remarks>
    /// NSFChoiceState is a transient state.  
    /// Once entered, it forces evaluation of all the outgoing transitions.
    /// All outgoing transition must have no triggers.
    /// There can be one and only one "else" transition, which has a null guard.
    /// Internal transitions state are not handled by transient states.  
    /// If no outgoing transition is taken, then the model is ill formed and an exception is thrown.
    /// </remarks>
    class NSFChoiceState : public NSFState
    {
    public:

        /// <summary>
        /// Creates a choice pseudo-state.
        /// </summary>
        /// <param name="name">The name of the choice pseudo-state.</param>
        /// <param name="parentRegion">The parent region of the choice pseudo-state.</param>
        NSFChoiceState(const NSFString& name, NSFRegion* parentRegion);

        /// <summary>
        /// Creates a choice pseudo-state.
        /// </summary>
        /// <param name="name">The name of the choice pseudo-state.</param>
        /// <param name="parentState">The parent state of the choice pseudo-state.</param>
        NSFChoiceState(const NSFString& name, NSFCompositeState* parentState);

    protected:

        virtual NSFEventStatus processEvent(NSFEvent* nsfEvent);
    };
}

#endif // NSF_CHOICE_STATE_H
