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

#include "CommandProcessorWithRetry.h"

namespace CommandProcessorWithRetryExample
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    CommandProcessorWithRetry::CommandProcessorWithRetry(const NSFString& name)
        : CommandProcessor(name), retries(0), maxRetries(1),
        waitForResponseReactionToResponseTimeout("WaitForResponseReactionToResponseTimeout", &waitForResponseState, &responseTimeoutEvent, NSFGuard(this, &CommandProcessorWithRetry::canRetry), NSFAction(this, &CommandProcessorWithRetry::retrySend))
    {
        waitForResponseState.EntryActions += NSFAction(this, &CommandProcessorWithRetry::waitForResponseEntryActions);
    }

    CommandProcessorWithRetry::~CommandProcessorWithRetry()
    {
        // It is good practice to stop event processing (terminate) in the 
        // destructor of a state machine or event handler to prevent 
        // entry/exit actions from being performed on a destructing object.
        terminate(true);
    }

    void CommandProcessorWithRetry::retrySend(const NSFStateMachineContext& context)
    {
        NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Retry send ..."));
        ++retries;
        // Code to re-send the command goes here
        // ...

        // Schedule timeout event, in case no response received           
        responseTimeoutEvent.schedule(responseTimeout, 0);
    }

    void CommandProcessorWithRetry::waitForResponseEntryActions(const NSFStateMachineContext& context)
    {
        retries = 0;
    }

    bool CommandProcessorWithRetry::canRetry(const NSFStateMachineContext& context)
    {
        return (retries < maxRetries);
    }
}