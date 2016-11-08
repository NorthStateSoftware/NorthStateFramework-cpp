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

#ifndef NSF_EVENT_THREAD_H
#define NSF_EVENT_THREAD_H

#include "NSFOSThread.h"
#include "NSFOSSignal.h"
#include "NSFThread.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a thread that has an event queue and dispatches events to their destinations.
    /// </summary>
    class NSFEventThread : public NSFThread
    {
        friend class NSFEventHandler;
        friend class NSFStateMachine;

    public:

        /// <summary>
        /// Creates an event thread.
        /// </summary>
        /// <param name="name">The name of the thread.</param>
        NSFEventThread(const NSFString& name);

        /// <summary>
        /// Creates an event thread.
        /// </summary>
        /// <param name="name">The name of the thread.</param>
        /// <param name="priority">The priority of the thread.</param>
        NSFEventThread(const NSFString& name, int priority);

        /// <summary>
        /// Destroys an event thread.
        /// </summary>
        /// <remarks>
        /// This destructor will block for a short period until the thread is terminated.
        /// </remarks>
        virtual ~NSFEventThread();

        /// <summary>
        /// Gets a list of event handlers using the thread.
        /// </summary>
        /// <returns>
        /// The list of event handlers using the thread.
        /// </returns>
        std::list<INSFEventHandler*> getEventHandlers();

        /// <summary>
        /// Indicates if the event queue contains an event that matches the specified event.
        /// </summary>
        /// <param name="nsfEvent">The event to match.</param>
        /// <returns>True if the event queue contains a matching event, false otherwise.</returns>
        /// <remarks>
        /// Two events match if they have the same id.  Events may be copied to create new events with the same id.
        /// </remarks>
        bool hasEvent(NSFEvent* nsfEvent);

        /// <summary>
        /// Indicates if the event queue contains an event for the specified destination.
        /// </summary>
        /// <param name="eventHandler">The event handler destination.</param>
        /// <returns>True if the event queue contains an event with the specified destination, false otherwise.</returns>
        bool hasEventFor(INSFEventHandler* eventHandler);

        /// <summary>
        /// Queues the specified event.
        /// </summary>
        /// <param name="nsfEvent">The event to queue.</param>
        /// <param name="isPriorityEvent">Flag indicating if the event should be queued to the back of the queue (false) or the front of the queue (true).</param>
        /// <param name="logEventQueued">Flag indicating if an event queued trace should be added to the trace log.</param>
        /// <remarks>
        /// Queueing a priority event to a state machine will invalidate its UML run-to-completion semantics.
        /// Use this feature for custom event handlers only.
        /// </remarks>
        void queueEvent(NSFEvent* nsfEvent, bool isPriorityEvent, bool logEventQueued);

        virtual void terminate(bool waitForTerminated);

    protected:

    private:

        std::list<NSFEvent*> nsfEvents;
        NSFOSSignal* signal;
        std::list<INSFEventHandler*> eventHandlers;

        /// <summary>
        /// Adds an event handler to the list of event handlers.
        /// </summary>
        void addEventHandler(INSFEventHandler* eventHandler);

        bool allEventHandlersTerminated();

        /// <summary>
        /// Clears all events in the event list.
        /// </summary>
        void clearEvents();

        /// <summary>
        /// Removes an event handler from the list of event handlers.
        /// </summary>
        void removeEventHandler(INSFEventHandler* eventHandler);

        virtual void threadLoop();
    };
}

#endif // NSF_EVENT_THREAD_H
