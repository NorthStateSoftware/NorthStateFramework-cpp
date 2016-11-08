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

#ifndef COMMANDW_PROCESSOR_WITH_RETRY_H
#define COMMANDW_PROCESSOR_WITH_RETRY_H

#include "CommandProcessor.h"

namespace CommandProcessorWithRetryExample
{
    class CommandProcessorWithRetry : public CommandProcessor
    {
    public:

        CommandProcessorWithRetry(const NSFString& name);
        ~CommandProcessorWithRetry();

    private:

        int retries;
        int maxRetries;

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Transitions, ordered internal, local, external
        NSFInternalTransition waitForResponseReactionToResponseTimeout;

        // State machine guards and actions
        void waitForResponseEntryActions(const NSFStateMachineContext& context);
        void retrySend(const NSFStateMachineContext& context);
        bool canRetry(const NSFStateMachineContext& context);
    };
}

#endif // COMMANDW_PROCESSOR_WITH_RETRY_H