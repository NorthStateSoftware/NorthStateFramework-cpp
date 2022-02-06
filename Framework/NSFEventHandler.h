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

#ifndef NSF_EVENT_HANDLER_H
#define NSF_EVENT_HANDLER_H

#include <map>

#include "NSFDelegates.h"
#include "NSFEvent.h"
#include "NSFTaggedTypes.h"
#include "NSFOSTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents the status of the event after attempting to handle it.
    /// </summary>
    /// <remarks>
    /// The status NSFEventHandled indicates that an action was taken as the result of the event.
    /// The status NSFEventUnhandled indicates that no action was taken as the result of the event.
    /// </remarks>
    enum NSFEventStatus { NSFEventUnhandled = 1, NSFEventHandled };

    /// <summary>
    /// Represents the run status of an event handler.
    /// </summary>
    /// <remarks>
    /// The run status indicates if the event handler is processing events in its queue.
    /// When the method getRunStatus() returns EventHandlerStarted, the event handler is processing events.
    /// Use the methods startEventHandler() and stopEventHandler() control event processing.
    /// </remarks>
    enum NSFEventHandlerRunStatus { EventHandlerStopped = 1, EventHandlerStarted };

    /// <summary>
    /// Represents the termination status of an event handler.
    /// </summary>
    /// <remarks>
    /// The termination status of an event handler can be used to determine when it is safe to delete.
    /// When the method getTerminationStatus() returns EventHandlerTerminated, it is safe to delete the event handler.
    /// Use the method terminate(...) to initiate the termination process.
    /// </remarks>
    enum NSFEventHandlerTerminationStatus { EventHandlerReady = 1, EventHandlerTerminating, EventHandlerTerminated };

#if (defined WIN32) || (defined WINCE)
    // Using virtual inheritance in its intended way, disable warning
#pragma warning( disable : 4250 )
#endif

    /// <summary>
    /// Represents the interface for classes that can queue and handle events.
    /// </summary>
    class INSFEventHandler : public virtual INSFNamedObject
    {
    public:

        /// <summary>
        /// Gets the run status of the event handler.
        /// </summary>
        /// <returns>The run status.</returns>
        /// <remarks>
        /// The run status indicates if the event handler is processing events in its queue.
        /// When the method getRunStatus() returns EventHandlerStarted, the event handler is processing events.
        /// Use the methods startEventHandler() and stopEventHandler() to control event processing.
        /// </remarks>
        virtual NSFEventHandlerRunStatus getRunStatus() const = 0;

        /// <summary>
        /// Gets the termination status of the event handler.
        /// </summary>
        /// <returns>The termination status.</returns>
        /// <remarks>
        /// The termination status of an event handler can be used to determine when it is safe to delete.
        /// When the method getTerminationStatus() returns EventHandlerTerminated, it is safe to delete the event handler.
        /// Use the method terminate(...) to initiate the termination process.
        /// </remarks>
        virtual NSFEventHandlerTerminationStatus getTerminationStatus() const = 0;

        /// <summary>
        /// Handles an event.
        /// </summary>
        /// <param name="nsfEvent">The event to handle.</param>
        /// <returns>Status indicating if the event was handled or not.</returns>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// </remarks>
        virtual NSFEventStatus handleEvent(NSFEvent* nsfEvent) = 0;

        /// <summary>
        /// Queues an event for the handler.
        /// </summary>
        /// <param name="nsfEvent">The event to queue.</param>
        virtual void queueEvent(NSFEvent* nsfEvent) = 0;

        /// <summary>
        /// Queues an event for the handler.
        /// </summary>
        /// <param name="nsfEvent">The event to queue.</param>
        /// <param name="source">The source of the event.</param>
        virtual void queueEvent(NSFEvent* nsfEvent, INSFNamedObject* source) = 0;

        /// <summary>
        /// Starts event processing.
        /// </summary>
        /// <remarks>
        /// When stopped, events queued prior to calling the method startEventHandler() will be dropped.
        /// Events queued after calling the method startEventHandler() will be processed, until the next call to stopEventHandler().
        /// When started, events queued prior to calling the method stopEventHandler() will be processed.
        /// Events queued after calling the method stopEventHandler() will be dropped, until the next call to startEventHandler().
        /// </remarks>
        virtual void startEventHandler() = 0;

        /// <summary>
        /// Stops event processing.
        /// </summary>
        /// <remarks>
        /// When stopped, events queued prior to calling the method startEventHandler() will be dropped.
        /// Events queued after calling the method startEventHandler() will be processed, until the next call to stopEventHandler().
        /// When started, events queued prior to calling the method stopEventHandler() will be processed.
        /// Events queued after calling the method stopEventHandler() will be dropped, until the next call to startEventHandler().
        /// </remarks>
        virtual void stopEventHandler() = 0;

        /// <summary>
        /// Terminates the event handler, so that it can be deleted.
        /// </summary>
        /// <param name="waitForTerminated">Flag indicating if the method should wait until the event handler is terminated (true), or if it should return immediately (false).</param>
        /// <remarks>
        /// This method terminates the event handler in a coordinated fashion,
        /// making sure all remaining events are cleared, so that it can be deleted.
        /// After calling this method, the event handler cannot be restarted.
        /// This method should only be called if the event handler is going to be deleted.
        /// If the waitForTerminated flag is set true, this method must not be called from the event handler's thread of execution.
        /// If the waitForTerminated flag is set false, the termination status of an event handler can be used to determine when it is safe to delete.
        /// When the method getTerminationStatus() returns EventHandlerTerminated, it is safe to delete the event handler.
        /// This method may be called multiple times without side effect.
        /// For example, an application may call terminate(false) on several event handlers to begin termination,
        /// and then call terminate(true) on each handler to wait for termination to complete.
        /// </remarks>
        virtual void terminate(bool waitForTerminated) = 0;
    };

    /// <summary>
    /// Represents a light-weight event handler.
    /// </summary>
    /// <remarks>
    /// This class associates actions with events, executing the actions when the event is handled.
    /// Events are queued to the specified thread and handled in the order they are received.
    /// Actions execute on the specified thread.
    /// This class is a light-weight event handler which can be used instead of a state machine.
    /// </remarks>
    class NSFEventHandler : public NSFTaggedObject, public INSFEventHandler
    {
    public:

        /// <summary>
        /// Creates an event handler
        /// </summary>
        /// <param name="name">The user defined name for the state machine.</param>
        /// <param name="thread">The thread on which events for the state machine are queued.</param>
        NSFEventHandler(const NSFString& name, NSFEventThread* thread);

        /// <summary>
        /// Destroys an event handler.
        /// </summary>
        virtual ~NSFEventHandler();

        virtual NSFEventHandlerRunStatus getRunStatus() const { return runStatus; }
        
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

        /// <summary>
        /// Gets the event handler's thread.
        /// </summary>
        NSFEventThread* getEventThread() const { return eventThread; }

        /// <summary>
        /// Gets the flag indicating if event logging is enabled.
        /// </summary>
        bool getLoggingEnabled() const { return loggingEnabled; }

        /// <summary>
        /// Sets the flag indicating if event logging is enabled.
        /// </summary>
        /// <param name="value">The value for the flag.</param>
        void setLoggingEnabled(bool value) { loggingEnabled = value; }

        /// <summary>
        /// Adds a reaction to a specified event.
        /// </summary>
        /// <param name="nsfEvent">The event causing the action.</param>
        /// <param name="action">The action taken as a result of the event.</param>
        void addEventReaction(NSFEvent* nsfEvent, const NSFVoidAction<NSFEventContext>& action);

        /// <summary>
        /// Handles an event.
        /// </summary>
        /// <param name="nsfEvent">The event to handle.</param>
        /// <returns>Status indicating if the event was handled or not.</returns>
        /// <remarks>
        /// This method is for use only by the North State Framework's internal logic.
        /// It calls the actions associated with the event, if any.
        /// </remarks>
        virtual NSFEventStatus handleEvent(NSFEvent* nsfEvent);

        /// <summary>
        /// Checks if an event is queued for handling.
        /// </summary>
        /// <param name="nsfEvent">The event in queustion.</param>
        /// <returns>True if the event is the in queue, otherwise false.</returns>
        bool hasEvent(NSFEvent* nsfEvent);

        /// <summary>
        /// Checks if any events are queued for handling.
        /// </summary>
        /// <returns>True if there are events queued for handling, otherwise false.</returns>
        bool hasEvent();

        virtual void queueEvent(NSFEvent* nsfEvent);

        virtual void queueEvent(NSFEvent* nsfEvent, INSFNamedObject* source);

        /// <summary>
        /// Removes a reaction to a specified event.
        /// </summary>
        /// <param name="nsfEvent">The event causing the action.</param>
        /// <param name="action">The action taken as a result of the event.</param>
        void removeEventReaction(NSFEvent* nsfEvent, const NSFVoidAction<NSFEventContext>& action);

        virtual void startEventHandler();

        virtual void stopEventHandler();

        virtual void terminate(bool waitForTerminated);

    private:

        bool loggingEnabled;

        std::map<NSFId, NSFVoidActions<NSFEventContext> > eventReactions;
        NSFEventThread* eventThread;
        NSFOSMutex* eventHandlerMutex;

        NSFEventHandlerRunStatus runStatus;
        NSFEventHandlerTerminationStatus terminationStatus;
        int terminationSleepTime;
        static int terminationTimeout;

        NSFEvent startEvent;
        NSFEvent stopEvent;
        NSFEvent terminateEvent;

        /// <summary>
        /// Handles exceptions caught while executing event actions.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void handleEventReactionException(const NSFExceptionContext& context);

        /// <summary>
        /// Handles exceptions raised during processing.
        /// </summary>
        /// <param name="exception">The exception thrown.</param>
        /// <remarks>
        /// The exception is forwarded to the global exception handler, NSFExceptionHandler.handleException().
        /// </remarks>
        void handleException(const std::exception& exception);
    };
}

#endif // NSF_EVENT_HANDLER_H
