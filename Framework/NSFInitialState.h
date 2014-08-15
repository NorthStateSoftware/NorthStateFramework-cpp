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

#ifndef NSF_INITIAL_STATE_H
#define NSF_INITIAL_STATE_H

#include "NSFState.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an initial pseudo-state.
    /// </summary>
    class NSFInitialState : public NSFState
    {
    public:

        /// <summary>
        /// Creates an initial pseudo-state.
        /// </summary>
        /// <param name="name">The name of the initial pseudo-state.</param>
        /// <param name="parentRegion">The parent region of the initial pseudo-state.</param>
        NSFInitialState(const NSFString& name, NSFRegion* parentRegion);

        /// <summary>
        /// Creates an initial pseudo-state.
        /// </summary>
        /// <param name="name">The name of the initial pseudo-state.</param>
        /// <param name="parentState">The parent state of the initial pseudo-state.</param>
        NSFInitialState(const NSFString& name, NSFCompositeState* parentState);
    };
}

#endif // NSF_INITIAL_STATE_H
