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

#ifndef NSF_DATA_EVENT_H
#define NSF_DATA_EVENT_H

#include "NSFEvent.h"
#include "NSFTimerAction.h"
#include "NSFTaggedTypes.h"
#include "NSFOSTypes.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents an event that contains a data payload.
    /// </summary>
    template<class DataType>
    class NSFDataEvent : public NSFEvent
    {
    public:

        /// <summary>
        /// Creates event that can have a data payload.
        /// </summary>
        /// <param name="name">The name of the event.</param>
        /// <param name="parent">The parent of the event.</param>
        /// <remarks>
        /// The event source and destination will be set to the parent.
        /// </remarks>
        NSFDataEvent(const NSFString& name, INSFEventHandler* parent)
            : NSFEvent(name, parent)
        {
        }

        /// <summary>
        /// Creates event that can have a data payload.
        /// </summary>
        /// <param name="name">The name of the event.</param>
        /// <param name="parent">The parent of the event.</param>
        /// <param name="data">The data of the event.</param>
        /// <remarks>
        /// The event source and destination will be set to the parent.
        /// </remarks>
        NSFDataEvent(const NSFString& name, INSFEventHandler* parent, const DataType& data)
            : NSFEvent(name, parent)
        {
            setData(data);
        }

        /// <summary>
        /// Creates event that can have a data payload.
        /// </summary>
        /// <param name="name">The name of the event.</param>
        /// <param name="source">The source of the event.</param>
        /// <param name="destination">The destination of event.</param>
        NSFDataEvent(const NSFString& name, INSFNamedObject* source, INSFEventHandler* destination)
            : NSFEvent(name, source, destination)
        {
        }

        /// <summary>
        /// Creates event that can have a data payload.
        /// </summary>
        /// <param name="name">The name of the event.</param>
        /// <param name="source">The source of the event.</param>
        /// <param name="destination">The destination of the event.</param>
        /// <param name="data">The data of the event.</param>
        NSFDataEvent(const NSFString& name, INSFNamedObject* source, INSFEventHandler* destination, const DataType& data)
            : NSFEvent(name, source, destination)
        {
            setData(data);
        }

        /// <summary>
        /// Creates event that can have a data payload.
        /// </summary>
        /// <param name="nsfEvent">The event to copy.</param>
        NSFDataEvent(const NSFDataEvent<DataType>& nsfEvent)
            : NSFEvent(nsfEvent)
        {
            setData(nsfEvent.data);
        }

        /// <summary>
        /// Creates event that can have a data payload.
        /// </summary>
        /// <param name="nsfEvent">The event to copy.</param>
        /// <param name="data">The data of the event.</param>
        NSFDataEvent(const NSFDataEvent<DataType>& nsfEvent, const DataType& data)
            : NSFEvent(nsfEvent)
        {
            setData(data);
        }

        /// <summary>
        /// Destroys a data event.
        /// </summary>
        virtual ~NSFDataEvent();

        /// <summary>
        /// Gets the data payload.
        /// </summary>
        /// <returns>The data payload</returns>
        DataType& getData() { return data; }

        /// <summary>
        /// Sets the data payload.
        /// </summary>
        /// <param name="value">The value for the data payload.</param>
        void setData(const DataType& value);

        /// <summary>
        /// Creates a deep copy, replacing the specified parameters.
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
        /// Creates a deep copy, replacing the specified parameters.
        /// </summary>
        /// <param name="deleteAfterHandling">Flag indicating if the event should be deleted after handled by an event handler.</param>
        /// <param name="data">The new data for the copy.</param>
        /// <returns>A copy of the event.</returns>
        /// <remarks>
        /// A common design pattern is to queue data event copies, each with its own unique data payload, to state machines for handling.
        /// The deleteAfterHandling flag is useful in this situation, because it allows new events to be created and queued to a state machine,
        /// letting the state machine delete the event once its handled.
        /// </remarks>
        virtual NSFDataEvent<DataType>* copy(bool deleteAfterHandling, const DataType& data);

        /// <summary>
        /// Creates a deep copy, replacing the specified parameters.
        /// </summary>
        /// <param name="name">The new name for the copy.</param>
        /// <param name="deleteAfterHandling">Flag indicating if the event should be deleted after handled by an event handler.</param>
        /// <param name="data">The new data for the copy.</param>
        /// <returns>A copy of the event.</returns>
        /// <remarks>
        /// A common design pattern is to queue data event copies, each with its own unique data payload, to state machines for handling.
        /// The deleteAfterHandling flag is useful in this situation, because it allows new events to be created and queued to a state machine,
        /// letting the state machine delete the event once its handled.
        /// </remarks>
        NSFDataEvent<DataType>* copy(const NSFString& name, bool deleteAfterHandling, const DataType& data);

        /// <summary>
        /// Creates a deep copy, replacing the specified parameters.
        /// </summary>
        /// <param name="source">The new source for the copy.</param>
        /// <param name="destination">The new destination for the copy.</param>
        /// <param name="deleteAfterHandling">Flag indicating if the event should be deleted after handled by an event handler.</param>
        /// <param name="data">The new data for the copy.</param>
        /// <returns>A copy of the event.</returns>
        /// <remarks>
        /// A common design pattern is to queue data event copies, each with its own unique data payload, to state machines for handling.
        /// The deleteAfterHandling flag is useful in this situation, because it allows new events to be created and queued to a state machine,
        /// letting the state machine delete the event once its handled.
        /// </remarks>
        NSFDataEvent<DataType>* copy(INSFNamedObject* source, INSFEventHandler* destination, bool deleteAfterHandling, const DataType& data);

        /// <summary>
        /// Creates a deep copy, replacing the specified parameters.
        /// </summary>
        /// <param name="name">The new name for the copy.</param>
        /// <param name="source">The new source for the copy.</param>
        /// <param name="destination">The new destination for the copy.</param>
        /// <param name="deleteAfterHandling">Flag indicating if the event should be deleted after handled by an event handler.</param>
        /// <param name="data">The new data for the copy.</param>
        /// <returns>A copy of the event.</returns>
        /// <remarks>
        /// A common design pattern is to queue data event copies, each with its own unique data payload, to state machines for handling.
        /// The deleteAfterHandling flag is useful in this situation, because it allows new events to be created and queued to a state machine,
        /// letting the state machine delete the event once its handled.
        /// </remarks>
        NSFDataEvent<DataType>* copy(const NSFString& name, INSFNamedObject* source, INSFEventHandler* destination, bool deleteAfterHandling, const DataType& data);

    private:

        DataType data;
    };

    // Template method definitions

    template<class DataType>
    NSFDataEvent<DataType>::~NSFDataEvent()
    {
    }

    template<class DataType>
    NSFEvent* NSFDataEvent<DataType>::copy(bool deleteAfterHandling)
    {
        NSFEvent* eventCopy = new NSFDataEvent<DataType>(*this);
        eventCopy->setDeleteAfterHandling(deleteAfterHandling);
        return eventCopy;
    }

    template<class DataType>
    NSFDataEvent<DataType>* NSFDataEvent<DataType>::copy(bool deleteAfterHandling, const DataType& data)
    {
        NSFDataEvent<DataType>* eventCopy = new NSFDataEvent<DataType>(*this, data);
        eventCopy->setDeleteAfterHandling(deleteAfterHandling);
        return eventCopy;
    }

    template<class DataType>
    NSFDataEvent<DataType>* NSFDataEvent<DataType>::copy(const NSFString& name, bool deleteAfterHandling, const DataType& data)
    {
        NSFDataEvent<DataType>* eventCopy = copy(deleteAfterHandling, data);
        eventCopy->setName(name);
        return eventCopy;
    }

    template<class DataType>
    NSFDataEvent<DataType>* NSFDataEvent<DataType>::copy(INSFNamedObject* source, INSFEventHandler* destination, bool deleteAfterHandling, const DataType& data)
    {
        NSFDataEvent<DataType>* eventCopy = copy(deleteAfterHandling, data);
        eventCopy->setSource(source);
        eventCopy->setDestination(destination);
        return eventCopy;
    }

    template<class DataType>
    NSFDataEvent<DataType>* NSFDataEvent<DataType>::copy(const NSFString& name, INSFNamedObject* source, INSFEventHandler* destination, bool deleteAfterHandling, const DataType& data)
    {
        NSFDataEvent<DataType>* eventCopy = copy(deleteAfterHandling, data);
        eventCopy->setName(name);
        eventCopy->setSource(source);
        eventCopy->setDestination(destination);
        return eventCopy;
    }

    // Defined here to work with both GCC and MSVC compilers

    template<class DataType>
    inline void NSFDataEvent<DataType>::setData(const DataType& value)
    {
        data = value;
    }

    // Specializations

    template<>
    inline void NSFDataEvent<NSFString>::setData(const NSFString& value)
    {
        data.assign(value.c_str());
    }
}

#endif // NSF_DATA_EVENT_H
