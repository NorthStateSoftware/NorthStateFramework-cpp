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

#ifndef NSF_EVENT_H
#define NSF_EVENT_H

#include "NSFStateMachineTypes.h"
#include "NSFTaggedTypes.h"
#include "NSFTimerAction.h"
#include "NSFOSTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an event which can trigger a transition or be handled by an event handler.
    /// </summary>
    class NSFEvent : public NSFTimerAction
    {
    public:

        /// <summary>
        /// Creates an event.
        /// </summary>
        /// <param name="name">The name of the event.</param>
        /// <param name="parent">The parent of the event.</param>
        /// <remarks>
        /// The event source and destination will be set to the parent.
        /// </remarks>
        NSFEvent(const NSFString& name, INSFEventHandler* parent);

        /// <summary>
        /// Creates an event.
        /// </summary>
        /// <param name="name">The name of the event.</param>
        /// <param name="source">The source of the event.</param>
        /// <param name="destination">The destination of the event.</param>
        NSFEvent(const NSFString& name, INSFNamedObject* source, INSFEventHandler* destination);

        /// <summary>
        /// Creates an event.
        /// </summary>
        /// <param name="nsfEvent">The event to copy.</param>
        NSFEvent(const NSFEvent& nsfEvent);

        /// <summary>
        /// Destroys an event.
        /// </summary>
        virtual ~NSFEvent();

        /// <summary>
        /// Gets the flag indicating whether the event should be deleted after handled by an event thread.
        /// </summary>
        bool getDeleteAfterHandling() const { return deleteAfterHandling; }

        /// <summary>
        /// Sets the flag indicating whether the event should be deleted after handled by an event thread.
        /// </summary>
        /// <param name="value">The value for the flag.</param>
        void setDeleteAfterHandling(bool value) { deleteAfterHandling = value; }

        /// <summary>
        /// Gets the event destination.
        /// </summary>
        INSFEventHandler* getDestination() const { return destination; }

        /// <summary>
        /// Sets the event destination.
        /// </summary>
        /// <param name="value">The event destination.</param>
        void setDestination(INSFEventHandler* value) { destination = value; }

        /// <summary>
        /// Gets the event id.
        /// </summary>
        /// <remarks>
        /// Transitions use the event id to match the trigger event.  In other words, mulitple event objects with the same id can trigger a transition.
        /// Use the copy() method to create a new event object with the same id as an existing object.
        /// </remarks>
        NSFId getId() const { return id; }

        /// <summary>
        /// Gets the event source.
        /// </summary>
        INSFNamedObject* getSource() const { return source; }

        /// <summary>
        /// Sets the event source.
        /// </summary>
        /// <param name="value">The event source.</param>
        void setSource(INSFNamedObject* value) { source = value; }

        /// <summary>
        /// Creates a deep copy.
        /// </summary>
        /// <param name="deleteAfterHandling">Flag indicating if the event should be deleted after handled by an event handler.</param>
        /// <returns>A copy of the event.</returns>
        /// <remarks>
        /// A common design pattern is to queue data event copies, each with its own unique data payload, to state machines for handling.
        /// The deleteAfterHandling flag is useful in this situation, because it allows new events to be created and queued to a state machine,
        /// letting the state machine delete the event once its handled.
        /// </remarks>
        virtual NSFEvent* copy(bool deleteAfterHandling);

        /// <summary>
        /// Creates a deep copy.
        /// </summary>
        /// <param name="name">The new name for the copy.</param>
        /// <param name="deleteAfterHandling">Flag indicating if the event should be deleted after handled by an event handler.</param>
        /// <returns>A copy of the event.</returns>
        /// <remarks>
        /// A common design pattern is to queue data event copies, each with its own unique data payload, to state machines for handling.
        /// The deleteAfterHandling flag is useful in this situation, because it allows new events to be created and queued to a state machine,
        /// letting the state machine delete the event once its handled.
        /// </remarks>
        NSFEvent* copy(const NSFString& name, bool deleteAfterHandling);

        /// <summary>
        /// Creates a deep copy, replacing the specified parameters.
        /// </summary>
        /// <param name="source">The new source for the copy.</param>
        /// <param name="destination">The new destination for the copy.</param>
        /// <param name="deleteAfterHandling">Flag indicating if the event should be deleted after handled by an event handler.</param>
        /// <returns>A copy of the event.</returns>
        /// <remarks>
        /// A common design pattern is to queue data event copies, each with its own unique data payload, to state machines for handling.
        /// The deleteAfterHandling flag is useful in this situation, because it allows new events to be created and queued to a state machine,
        /// letting the state machine delete the event once its handled.
        /// </remarks>
        NSFEvent* copy(INSFNamedObject* source, INSFEventHandler* destination, bool deleteAfterHandling);

        /// <summary>
        /// Creates a deep copy, replacing the specified parameters.
        /// </summary>
        /// <param name="name">The new name for the copy.</param>
        /// <param name="source">The new source for the copy.</param>
        /// <param name="destination">The new destination for the copy.</param>
        /// <param name="deleteAfterHandling">Flag indicating if the event should be deleted after handled by an event handler.</param>
        /// <returns>A copy of the event.</returns>
        /// <remarks>
        /// A common design pattern is to queue data event copies, each with its own unique data payload, to state machines for handling.
        /// The deleteAfterHandling flag is useful in this situation, because it allows new events to be created and queued to a state machine,
        /// letting the state machine delete the event once its handled.
        /// </remarks>
        NSFEvent* copy(const NSFString& name, INSFNamedObject* source, INSFEventHandler* destination, bool deleteAfterHandling);

        /// <summary>
        /// Queues the event to its destination.
        /// </summary>
        void queueEvent();

        /// <summary>
        /// Queues the event to its destination.
        /// </summary>
        /// <remarks>
        /// This method is provided to allow event queuing to be registered as an entry, exit, or transition action.
        /// The event source will be changed to the context source for logging purposes during execution of this method.
        /// This method is not thread safe from a logging perspective, so that two threads calling this method on the same
        /// event can result in indeterminate logging of the source.
        /// </remarks>
        void queueEvent(const NSFStateMachineContext& context);

        /// <summary>
        /// Schedules the event to execute.
        /// </summary>
        /// <param name="delayTime">Delay time before executing the event.</param>
        /// <param name="repeatTime">Repeat time, if desired.  Zero if one-shot.</param>
        void schedule(NSFTime delayTime, NSFTime repeatTime);

        /// <summary>
        /// Schedules the event to execute.
        /// </summary>
        /// <param name="source">Source of the event.</param>
        /// <param name="destination">Destination of the event.</param>
        /// <param name="delayTime">Delay time before executing the event.</param>
        /// <param name="repeatTime">Repeat time, if desired.  Zero if one-shot.</param>
        void schedule(INSFNamedObject* source, INSFEventHandler* destination, NSFTime delayTime, NSFTime repeatTime);

        /// <summary>
        /// Sets the source and destination of the event.
        /// </summary>
        /// <param name="source">Source of the event.</param>
        /// <param name="destination">Destination of the event.</param>
        void setRouting(INSFNamedObject* source, INSFEventHandler* destination);

    private:

        bool deleteAfterHandling;

        NSFId id;
        INSFNamedObject* source;
        INSFEventHandler* destination;

        /// <summary>
        /// Callback method supporting NSFTimerAction interface.
        /// </summary>
        /// <remarks>
        /// This method is called by the NSFTimerThread to queue the event at the scheduled time.
        /// </remarks>
        virtual void execute();
    };
}

#endif // NSF_EVENT_H
