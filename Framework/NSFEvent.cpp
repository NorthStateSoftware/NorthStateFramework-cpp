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

#include "NSFEvent.h"

#include "NSFEventHandler.h"
#include "NSFStateMachine.h"
#include "NSFTimerThread.h"

namespace NorthStateFramework
{
    // Public

    NSFEvent::NSFEvent(const NSFString& name, INSFEventHandler* parent)
        : NSFTimerAction(name), deleteAfterHandling(false), id(NSFUniquelyNumberedObject::getNextUniqueId()), source(parent), destination(parent)
    {
    }

    NSFEvent::NSFEvent(const NSFString& name, INSFNamedObject* source, INSFEventHandler* destination)
        : NSFTimerAction(name), deleteAfterHandling(false), id(NSFUniquelyNumberedObject::getNextUniqueId()), source(source), destination(destination)
    {
    }

    NSFEvent::NSFEvent(const NSFEvent& nsfEvent)
        : NSFTimerAction(nsfEvent.getName()), deleteAfterHandling(false), id(nsfEvent.getId()), source(nsfEvent.getSource()), destination(nsfEvent.getDestination())
    {
    }

    NSFEvent::~NSFEvent()
    {
    }

    NSFEvent* NSFEvent::copy(bool deleteAfterHandling)
    {
        NSFEvent* eventCopy = new NSFEvent(*this);
        eventCopy->setDeleteAfterHandling(deleteAfterHandling);
        return eventCopy;
    }

    NSFEvent* NSFEvent::copy(const NSFString& name, bool deleteAfterHandling)
    {
        NSFEvent* eventCopy = copy(deleteAfterHandling);
        eventCopy->setName(name);
        return eventCopy;
    }

    NSFEvent* NSFEvent::copy(INSFNamedObject* source, INSFEventHandler* destination, bool deleteAfterHandling)
    {
        NSFEvent* eventCopy = copy(deleteAfterHandling);
        eventCopy->setSource(source);
        eventCopy->setDestination(destination);
        return eventCopy;
    }

    NSFEvent* NSFEvent::copy(const NSFString& name, INSFNamedObject* source, INSFEventHandler* destination, bool deleteAfterHandling)
    {
        NSFEvent* eventCopy = copy(deleteAfterHandling);
        eventCopy->setName(name);
        eventCopy->setSource(source);
        eventCopy->setDestination(destination);
        return eventCopy;
    }

    void NSFEvent::queueEvent()
    {
        destination->queueEvent(this);
    }

    void NSFEvent::queueEvent(const NSFStateMachineContext& context)
    {
        destination->queueEvent(this, context.getSource());
    }

    void NSFEvent::schedule(NSFTime delayTime, NSFTime repeatTime)
    {
        NSFTimerThread::getPrimaryTimerThread().scheduleAction(this, delayTime, repeatTime);
    }

    void NSFEvent::schedule(INSFNamedObject* source, INSFEventHandler* destination, NSFTime delayTime, NSFTime repeatTime)
    {
        setSource(source);
        setDestination(destination);
        NSFTimerThread::getPrimaryTimerThread().scheduleAction(this, delayTime, repeatTime);
    }

    void NSFEvent::setRouting(INSFNamedObject* source, INSFEventHandler* destination)
    {
        setSource(source);
        setDestination(destination);
    }

    // Private

    void NSFEvent::execute()
    {
        destination->queueEvent(this);
    }
}
