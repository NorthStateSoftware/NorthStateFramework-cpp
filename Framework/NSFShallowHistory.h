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

#ifndef NSF_SHALLOW_HISTORY_H
#define NSF_SHALLOW_HISTORY_H

#include "NSFState.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a shallow history pseudo-state.
    /// </summary>
    /// <remarks>
    /// NSFShallowHistory is a transient state.
    /// Once entered, it forces entry into the last active substate in the region.
    /// </remarks>
    class NSFShallowHistory : public NSFState
    {
    public:

        /// <summary>
        /// Creates a shallow history pseudo-state.
        /// </summary>
        /// <param name="name">The name of the shallow history pseudo-state.</param>
        /// <param name="parentRegion">The parent region of the shallow history pseudo-state.</param>
        NSFShallowHistory(const NSFString& name, NSFRegion* parentRegion);

        /// <summary>
        /// Creates a shallow history pseudo-state.
        /// </summary>
        /// <param name="name">The name of the shallow history pseudo-state.</param>
        /// <param name="parentState">The parent state of the shallow history pseudo-state.</param>
        NSFShallowHistory(const NSFString& name, NSFCompositeState* parentState);

    protected:

        virtual void enter(NSFStateMachineContext& context, bool useHistory);
    };
}

#endif // NSF_SHALLOW_HISTORY_H
