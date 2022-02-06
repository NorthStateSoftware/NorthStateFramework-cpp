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

    void CommandProcessorWithRetry::retrySend(const NSFStateMachineContext&)
    {
        NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Retry send ..."));
        ++retries;
        // Code to re-send the command goes here
        // ...

        // Schedule timeout event, in case no response received           
        responseTimeoutEvent.schedule(responseTimeout, 0);
    }

    void CommandProcessorWithRetry::waitForResponseEntryActions(const NSFStateMachineContext&)
    {
        retries = 0;
    }

    bool CommandProcessorWithRetry::canRetry(const NSFStateMachineContext&)
    {
        return (retries < maxRetries);
    }
}