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