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

#ifndef NSF_STATE_MACHINE_H
#define NSF_STATE_MACHINE_H

#include "NSFCompositeState.h"
#include "NSFEventHandler.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a state machine.
    /// </summary>
    class NSFStateMachine : public NSFCompositeState, public INSFEventHandler
    {
    public:

        friend class NSFState;
        friend class NSFTransition;

        /// <summary>
        /// Creates a state machine
        /// </summary>
        /// <param name="name">The user defined name for the state machine.</param>
        /// <param name="thread">The thread on which events for the state machine are queued.</param>
        NSFStateMachine(const NSFString& name, NSFEventThread* thread);

        /// <summary>
        /// Creates a state machine.
        /// </summary>
        /// <param name="name">The user defined name for the state machine.</param>
        /// <param name="parentRegion">The parent region of the state machine.</param>
        NSFStateMachine(const NSFString& name, NSFRegion* parentRegion);

        /// <summary>
        /// Creates a state machine.
        /// </summary>
        /// <param name="name">The user defined name for the state machine.</param>
        /// <param name="parentState">The parent composite state for the state machine.</param>
        NSFStateMachine(const NSFString& name, NSFCompositeState* parentState);

        /// <summary>
        /// Destroys a state machine.
        /// </summary>
        virtual ~NSFStateMachine();

        /// <summary>
        /// Actions to execute if an exception is encountered by the state machine.
        /// </summary>
        NSFVoidActions<NSFExceptionContext> ExceptionActions;

        /// <summary>
        /// Actions to execute whenever a new state is entered.
        /// </summary>
        NSFVoidActions<NSFStateMachineContext> StateChangeActions;

        /// <summary>
        /// Gets the flag indicating if consecutive loop detection is enabled.
        /// </summary>
        /// <remarks>
        /// State machines have the ability to detect if they are ill-formed by way of consecutive loop detection.
        /// This mechanism looks for repeated transitions without a pause (as defined by the condition when there
        /// are no more events queued to the state machine, and the final run-to-completion step has occurred).
        /// If more than the specified number of transitions occur without a pause, the state machine will automatically stop.
        /// </remarks>
        bool getConsecutiveLoopDetectionEnabled() const { return consecutiveLoopDetectionEnabled; }

        /// <summary>
        /// Sets the flag indicating if consecutive loop detection is enabled.
        /// </summary>
        /// <remarks>
        /// State machines have the ability to detect if they are ill-formed by way of consecutive loop detection.
        /// This mechanism looks for repeated transitions without a pause (as defined by the condition when there
        /// are no more events queued to the state machine, and the final run-to-completion step has occurred).
        /// If more than the specified number of transitions occur without a pause, the state machine will automatically stop.
        /// </remarks>
        void setConsecutiveLoopDetectionEnabled(bool value) { consecutiveLoopDetectionEnabled = value; }

        /// <summary>
        /// Gets the maximum number of transitions without a pause allowed by consecutive loop detection.
        /// </summary>
        /// <remarks>
        /// State machines have the ability to detect if they are ill-formed by way of consecutive loop detection.
        /// This mechanism looks for repeated transitions without a pause (as defined by the condition when there
        /// are no more events queued to the state machine, and the final run-to-completion step has occurred).
        /// If more than the specified number of transitions occur without a pause, the state machine will automatically stop.
        /// </remarks>
        int getConsecutiveLoopLimit() const { return consecutiveLoopLimit; }

        /// <summary>
        /// Sets the maximum number of transitions without a pause allowed by consecutive loop detection.
        /// </summary>
        /// <remarks>
        /// State machines have the ability to detect if they are ill-formed by way of consecutive loop detection.
        /// This mechanism looks for repeated transitions without a pause (as defined by the condition when there
        /// are no more events queued to the state machine, and the final run-to-completion step has occurred).
        /// If more than the specified number of transitions occur without a pause, the state machine will automatically stop.
        /// </remarks>
        void setConsecutiveLoopLimit(int value) { consecutiveLoopLimit = value; }

        /// <summary>
        ///  Provides a syntactical method for specifying "Else" as a transition guard
        /// </summary>
        static const NSFBoolGuard<NSFStateMachineContext>* Else;

        /// <summary>
        /// Gets the state machine's thread.
        /// </summary>
        /// <returns>The state machine's thread.</returns>
        NSFEventThread* getEventThread() const { return eventThread; }

        /// <summary>
        /// Gets the flag indicating if state machine event logging is enabled.
        /// </summary>
        /// <returns>The value of the flag.</returns>
        bool getLoggingEnabled() const { return loggingEnabled; }

        /// <summary>
        /// Sets flag indicating if state machine event logging is enabled.
        /// </summary>
        /// <param name="value">The value for the flag.</param>
        void setLoggingEnabled(bool value) { loggingEnabled = value; }

        virtual NSFEventHandlerTerminationStatus getTerminationStatus() const { return terminationStatus; }

        /// <summary>
        /// Gets the amount of time (mS) the <see cref="terminate"/> method sleeps between checks
        /// for event handler termination.
        /// </summary>
        int getTerminationSleepTime() { return terminationSleepTime; }

        /// <summary>
        /// Sets the amount of time (mS) the <see cref="terminate"/> method sleeps between checks
        /// for event handler termination.
        /// </summary>
        void setTerminationSleepTime(int value) { terminationSleepTime = value; }

        /// <summary>
        /// Gets the amount of time the <see cref="terminate"/> method will wait for event processing to complete.
        /// </summary>
        static int getTerminationTimeout() { return terminationTimeout; }

        /// <summary>
        /// Sets the amount of time the <see cref="terminate"/> method will wait for event processing to complete.
        /// </summary>
        static void setTerminationTimeout(int value) { terminationTimeout = value; }

        virtual NSFEventHandlerRunStatus getRunStatus() const { return runStatus; }

        virtual NSFStateMachine* getTopStateMachine();

        /// <summary>
        /// Forces the state machine to evaluate transitions.
        /// </summary>
        /// <remarks>
        /// This method is rarely needed, but can be used, for example, to force evaluation if a guard condition has changed.
        /// </remarks>
        void forceStateMachineEvaluation();

        /// <summary>
        /// Handles an event.
        /// </summary>
        /// <param name="nsfEvent">The event to handle.</param>
        /// <returns>Status indicating if the event was handled or not.</returns>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// It processes the event using UML defined behavior, including run to completion.
        /// </remarks>
        virtual NSFEventStatus handleEvent(NSFEvent* nsfEvent);

        virtual void queueEvent(NSFEvent* nsfEvent);

        virtual void queueEvent(NSFEvent* nsfEvent, INSFNamedObject* source);

        /// <summary>
        /// Resets the state machine back to its initial default state.
        /// </summary>
        /// <remarks>
        /// If the state machine is currently running (started),
        /// it will process all events currently in its queue before resetting and
        /// it will continue running after the reset.
        /// If the state machine is currently stopped, it will continue to be stopped after the reset.
        /// While a state machine is stopped, all events are dropped from its queue.
        /// </remarks>
        void resetStateMachine();

        virtual void startEventHandler();

        virtual void stopEventHandler();

        virtual void startStateMachine();

        virtual void stopStateMachine();

        virtual void terminate(bool waitForTerminated);

    protected:

        /// <summary>
        /// Handles exceptions raised during state machine processing.
        /// </summary>
        /// <param name="exception">The exception thrown.</param>
        /// <remarks>
        /// By default, any exception actions are executed first,
        /// then the exception is forwarded to the global exception handler, NSFExceptionHandler.handleException().
        /// </remarks>
        void handleException(const std::exception& exception);

        /// <summary>
        /// Checks if an event is queued for processing.
        /// </summary>
        /// <param name="nsfEvent">The event in queustion.</param>
        /// <returns>True if the event is the in queue, otherwise false.</returns>
        bool hasEvent(NSFEvent* nsfEvent);

        /// <summary>
        /// Checks if any events are queued for processing.
        /// </summary>
        /// <returns>True if there are events queued for processing, otherwise false.</returns>
        bool hasEvent();

        /// <summary>
        /// Checks if the state machine is the top state machine in a nested heirarchy.
        /// </summary>
        /// <returns>True if the state machine is the top state machine, otherwise false.</returns>
        bool isTopStateMachine();

        /// <summary>
        /// Re-routes a transition between a specified source and destination
        /// </summary>
        /// <param name="transition">The transition to re-route.</param>
        /// <param name="source">The new source for the transition.</param>
        /// <param name="target">The new target for the transition.</param>
        /// <remarks>
        /// This method can be used when extending state machines.
        /// Careful design consideration should be made before using this method.
        /// Never change a state machines structure while it is running.
        /// </remarks>
        void rerouteTransition(NSFTransition* transition, NSFState* source, NSFState* target);

        virtual void reset();

    private:

        int consecutiveLoopCount;
        bool consecutiveLoopDetectionEnabled;
        int consecutiveLoopLimit;
        bool loggingEnabled;

        NSFEventThread* eventThread;
        NSFOSMutex* stateMachineMutex;

        NSFEventHandlerRunStatus runStatus;
        NSFEventHandlerTerminationStatus terminationStatus;
        int terminationSleepTime;
        static int terminationTimeout;

        NSFEvent resetEvent;
        NSFEvent runToCompletionEvent;
        NSFEvent startEvent;
        NSFEvent stopEvent;
        NSFEvent terminateEvent;

        /// <summary>
        /// Performs common construction behaviors.
        /// </summary>
        void construct();

        /// <summary>
        /// Executes the actions in the state change actions list.
        /// </summary>
        void executeStateChangeActions(NSFStateMachineContext& context);

        /// <summary>
        /// Handles exceptions caught while executing state change actions.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void handleStateChangeActionException(const NSFExceptionContext& context);

        /// <summary>
        /// Adds the specified event to the state machine's event queue.
        /// </summary>
        /// <param name="nsfEvent">The event to queue.</param>
        /// <param name="isPriorityEvent">Flag indicating if the event should be queued to the back of the queue (false) or the front of the queue (true).</param>
        /// <param name="logEventQueued">Flag indicating if an event queued trace should be added to the trace log.</param>
        void queueEvent(NSFEvent* nsfEvent, bool isPriorityEvent, bool logEventQueued);

        /// <summary>
        /// Forces the state machine to run to completion.
        /// </summary>
        void runToCompletion();
    };
}

#endif // NSF_STATE_MACHINE_H
