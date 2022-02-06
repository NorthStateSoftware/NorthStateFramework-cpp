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

#ifndef NSF_DELEGATE_CONTEXT_H
#define NSF_DELEGATE_CONTEXT_H

#include "NSFEvent.h"
#include <stdexcept>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a general purpose action or guard context.
    /// </summary>
    class NSFContext
    {
    public:

        /// <summary>
        /// Creates a general purpose action or guard context.
        /// </summary>
        /// <param name="source">The source of invocation.</param>
        NSFContext(void* source)
            : source(source)
        {
        }

        /// <summary>
        /// Gets the source of invocation.
        /// </summary>
        void* getSource() const { return source; }

    private:

        void* source;
    };

    /// <summary>
    /// Represents contextual information for NSF event actions.
    /// </summary>
    class NSFEventContext : public NSFContext
    {
    public:

        /// <summary>
        /// Creates an NSF event context.
        /// </summary>
        /// <param name="source">The source of invocation.</param>
        /// <param name="nsfEvent">The event.</param>
        NSFEventContext(void* source, NSFEvent* nsfEvent)
            : NSFContext(source), nsfEvent(nsfEvent)
        {
        }

        /// <summary>
        /// Gets the NSF event.
        /// </summary>
        NSFEvent* getEvent() const { return nsfEvent; }

    private:

        NSFEvent* nsfEvent;
    };

    /// <summary>
    /// Represents contextual information for exception actions.
    /// </summary>
    class NSFExceptionContext : public NSFContext
    {
    public:

        /// <summary>
        /// Creates an exception context.
        /// </summary>
        /// <param name="source">The source of invocation.</param>
        /// <param name="exception">The exception that occurred.</param>
        NSFExceptionContext(void* source, const std::exception& exception)
            : NSFContext(source), exception(exception)
        {
        }

        /// <summary>
        /// Gets the exception.
        /// </summary>
        const std::exception& getException() const { return exception; }

    private:

        // Make non-assignable
        NSFExceptionContext& operator=(const NSFExceptionContext&);

        const std::exception& exception;
    };

    /// <summary>
    /// Represents contextual information for state machine actions and guards.
    /// </summary>
    class NSFStateMachineContext : public NSFContext
    {
    public:

        friend class NSFExternalTransition;
        friend class NSFLocalTransition;
        friend class NSFState;

        /// Creates a state machine context.
        /// </summary>
        /// <param name="source">The state machine.</param>
        /// <param name="enteringState">The state being entered.</param>
        /// <param name="exitingState">The state being exited.</param>
        /// <param name="transition">The associated transition.</param>
        /// <param name="trigger">The triggering event.</param>
        NSFStateMachineContext(NSFStateMachine* source, NSFState* enteringState, NSFState* exitingState, NSFTransition* transition, NSFEvent* trigger)
            : NSFContext(source), enteringState(enteringState), exitingState(exitingState), transition(transition), trigger(trigger)
        {
        }

        /// <summary>
        /// Gets the state being entered.
        /// </summary>
        /// <remarks>
        /// Entering state is null unless the context corresponds to an entry action.
        /// </remarks>
        NSFState* getEnteringState() const { return enteringState; }

        /// <summary>
        /// Gets the state being exited.
        /// </summary>
        /// <remarks>
        /// Exiting state is null unless the context corresponds to an exit action.
        /// </remarks>
        NSFState* getExitingState() const { return exitingState; }

        /// <summary>
        /// Gets the source of invocation.
        /// </summary>
        NSFStateMachine* getSource() const { return (NSFStateMachine*) NSFContext::getSource(); }

        /// <summary>
        /// Gets the associated transition.
        /// </summary>
        NSFTransition* getTransition() const { return transition; }

        /// <summary>
        /// Gets the triggering event.
        /// </summary>
        NSFEvent* getTrigger() const { return trigger; }

    private:

        NSFState* enteringState;
        NSFState* exitingState;
        NSFTransition* transition;
        NSFEvent* trigger;

        /// <summary>
        /// Sets the state being entered.
        /// </summary>
        /// <param name="value">The state being entered.</param>
        void setEnteringState(NSFState* value) { enteringState = value; }

        /// <summary>
        /// Sets the state being exited.
        /// </summary>
        /// <param name="value">The state being exited.</param>
        void setExitingState(NSFState* value) { exitingState = value; }
    };
}

#endif // NSF_DELEGATE_CONTEXT_H
