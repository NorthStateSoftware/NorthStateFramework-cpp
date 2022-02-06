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

#ifndef NSF_FORK_JOIN_TRANSITION_H
#define NSF_FORK_JOIN_TRANSITION_H

#include "NSFExternalTransition.h"
#include "NSFForkJoin.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a transition between two fork-joins.
    /// </summary>
    /// <remarks>
    /// This class is an extension to UML 2.x.  It allows a transition to be made between two fork-joins,
    /// optionally specifying a region associated with the transition.  If a region is specified, its
    /// active substate will be the target fork-join after the transition is taken.  If a NULL region is
    /// specified, then no region's active substate will be the target fork-join as a result of taking 
    /// this transition.  This latter case, where the associated region is NULL, is equivalent to using
    /// an external transition between the two fork-joins.
    /// </remarks>
    class NSFForkJoinTransition : public NSFExternalTransition
    {
    public:

        friend class NSFForkJoin;

        /// <summary>
        /// Creates a transition between two fork-joins.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="region">Transition region.</param>
        /// <param name="action">Transition action.</param>
        /// <remarks>
        /// This class is an extension to UML 2.x.  It allows a transition to be made between two fork-joins,
        /// optionally specifying a region associated with the transition.  If a region is specified, its
        /// active substate will be the target fork-join after the transition is taken.  If a NULL region is
        /// specified, then no region's active substate will be the target fork-join as a result of taking 
        /// this transition.  This latter case, where the associated region is NULL, is equivalent to using
        /// an external transition between the two fork-joins.
        /// </remarks>
        NSFForkJoinTransition(const NSFString& name, NSFForkJoin* source, NSFForkJoin* target, NSFRegion* region, const NSFVoidAction<NSFStateMachineContext>& action);

        /// <summary>
        /// Creates a transition between two fork-joins.
        /// </summary>
        /// <param name="name">User assigned name for transition.</param>
        /// <param name="source">Transition source.</param>
        /// <param name="target">Transition target.</param>
        /// <param name="region">Transition region.</param>
        /// <param name="action">Transition action.</param>
        /// <remarks>
        /// This class is an extension to UML 2.x.  It allows a transition to be made between two fork-joins,
        /// optionally specifying a region associated with the transition.  If a region is specified, its
        /// active substate will be the target fork-join after the transition is taken.  If a NULL region is
        /// specified, then no region's active substate will be the target fork-join as a result of taking 
        /// this transition.  This latter case, where the associated region is NULL, is equivalent to using
        /// an external transition between the two fork-joins.
        /// </remarks>
        NSFForkJoinTransition(const NSFString& name, NSFForkJoin* source, NSFForkJoin* target, NSFRegion* region, const NSFVoidAction<NSFStateMachineContext>* action);

    private:

        NSFRegion* forkJoinRegion;

        NSFRegion* getForkJoinRegion() const { return forkJoinRegion; }
    };
}

#endif // NSF_FORK_JOIN_TRANSITION_H
