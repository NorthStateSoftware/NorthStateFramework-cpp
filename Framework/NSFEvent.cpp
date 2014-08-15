// Copyright 2004-2014, North State Software, LLC.  All rights reserved.

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

    void NSFEvent::queueEvent(const NSFContext& context)
    {
        queueEvent();
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
