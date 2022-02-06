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

#ifndef NSF_FORK_JOIN_H
#define NSF_FORK_JOIN_H

#include "NSFState.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a fork-join state.
    /// </summary>
    /// <remarks>
    /// Fork-join states are used to provide synchronization and branching across multiple regions.
    /// </remarks>
    class NSFForkJoin : public NSFState
    {
    public:

        /// <summary>
        /// Creates a fork-join state.
        /// </summary>
        /// <param name="name">The name of the fork-join state.</param>
        /// <param name="parentState">The parent state of the fork-join state.</param>
        NSFForkJoin(const NSFString& name, NSFCompositeState* parentState);

        /// <summary>
        /// Indicates if the fork-join is the active substate within the specified region.
        /// </summary>
        /// <param name="region">The region in question.</param>
        /// <returns>True if the fork-join is the active substate, otherwise false.</returns>
        bool isActive(NSFRegion* region);

    protected:

        virtual NSFState* getParentState();

        virtual void enter(NSFStateMachineContext& context, bool useHistory);

        virtual void exit(NSFStateMachineContext& context);

        virtual NSFEventStatus processEvent(NSFEvent* nsfEvent);

        virtual void reset();

    private:

        std::list<NSFTransition*> completedTransitions;
        NSFCompositeState* parentState;
    };
}

#endif // NSF_FORK_JOIN_H
