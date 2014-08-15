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

#include "NSFTraceLog.h"

#include "NSFEventThread.h"
#include "NSFExceptionHandler.h"
#include "NSFTimerThread.h"

namespace NorthStateFramework
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    // Public

    NSFTraceLog::NSFTraceLog(const NSFString& name)
        : NSFTaggedObject(name), enabled(false), eventHandler(name, new NSFEventThread(name, NSFOSThread::getLowestPriority())),
        maxTraces(500), traceAddEvent(TraceAddString(), &eventHandler), traceLogMutex(NSFOSMutex::create()),
        traceSaveEvent(TraceSaveString(), &eventHandler), xmlLog(TraceLogTag())
    {
        eventHandler.setLoggingEnabled(false);

        eventHandler.addEventReaction(&traceAddEvent, NSFAction(this, &NSFTraceLog::addTraceToLog));
        eventHandler.addEventReaction(&traceSaveEvent, NSFAction(this, &NSFTraceLog::saveTrace));

        eventHandler.startEventHandler();

        addTrace(NSFTraceTags::InformationalTag(), NSFTraceTags::NameTag(), "TraceStart");
    }

    NSFTraceLog::~NSFTraceLog()
    {
        eventHandler.terminate(true);
        delete eventHandler.getEventThread();
        delete traceLogMutex;
    }

    void NSFTraceLog::addTrace(const NSFString& type, const NSFString& tag, const NSFString& data)
    {
        addTrace(type, tag, data, emptyString(), emptyString(), emptyString(), emptyString());
    }

    void NSFTraceLog::addTrace(const NSFString& type, const NSFString& tag1, const NSFString& data1, const NSFString& tag2, const NSFString& data2)
    {
        addTrace(type, tag1, data1, tag2, data2, emptyString(), emptyString());
    }

    void NSFTraceLog::addTrace(const NSFString& type, const NSFString& tag1, const NSFString& data1, const NSFString& tag2, const NSFString& data2, const NSFString& tag3, const NSFString& data3)
    {
        // If logging is not enabled, then return without action
        if (!enabled)
        {
            return;
        }

        NSFXMLElement* trace = NULL;

        try
        {
            // Lock to prevent events from being added to the queue with timestamps out of order
            LOCK(traceLogMutex)
            {
                // Create the new trace element
                trace = new NSFXMLElement(TraceTag());

                // Add the time
                trace->addChildElementBack(new NSFXMLElement(TimeTag(), toString(NSFTimerThread::getPrimaryTimerThread().getCurrentTime())));

                // Add a trace for the type
                NSFXMLElement* typeTrace = new NSFXMLElement(type);
                trace->addChildElementBack(typeTrace);

                // Add the new data under the type element;
                typeTrace->addChildElementBack(new NSFXMLElement(tag1, data1));

                if (!tag2.empty()) typeTrace->addChildElementBack(new NSFXMLElement(tag2, data2));
                if (!tag3.empty()) typeTrace->addChildElementBack(new NSFXMLElement(tag3, data3));

                eventHandler.queueEvent(traceAddEvent.copy(true, trace));
            }
            ENDLOCK;
        }
        catch(...)
        {
            // If unable to add a trace, just do nothing, because calling the exception handler may result in an infinite loop
            delete trace;
        }
    }

    bool NSFTraceLog::allTracesLogged()
    {
        return !eventHandler.hasEvent();
    }

    void NSFTraceLog::saveLog()
    {
        saveLog(getName());
    }

    void NSFTraceLog::saveLog(const NSFString& fileName)
    {
        if (enabled)
        {
            traceSaveEvent.copy(true, fileName)->queueEvent();
        }
    }

    // Private

    void NSFTraceLog::addTraceToLog(const NSFEventContext& context)
    {
        try
        {
            xmlLog.jumpToRootElement();

            UInt32 numberOfChildElements;
            while (xmlLog.getNumberOfChildElements(numberOfChildElements) && (numberOfChildElements >= maxTraces))
            {
                xmlLog.deleteChildElementFront();
            }

            xmlLog.addChildElementBack(((NSFDataEvent<NSFXMLElement*>*)context.getEvent())->getData());
        }
        catch (...)
        {
            // If unable to add a trace, just do nothing, because calling the exception handler may result in an infinite loop
        }
    }

    void NSFTraceLog::saveTrace(const NSFEventContext& context)
    {
        try
        {
            addTrace(NSFTraceTags::InformationalTag(), NSFTraceTags::NameTag(), TraceSaveString());
            xmlLog.save(((NSFDataEvent<NSFString>*)context.getEvent())->getData());
            addTrace(NSFTraceTags::InformationalTag(), NSFTraceTags::NameTag(), TraceSaveCompleteString());
        }
        catch (const std::exception& exception)
        {
            // If unable to save a trace, just add a trace with the exception message, because calling the exception handler may result in an infinite loop
            addTrace(NSFTraceTags::ExceptionTag(), NSFTraceTags::MessageTag(), getName() + " exception saving trace: " + exception.what());
        }
        catch (...)
        {
            addTrace(NSFTraceTags::ExceptionTag(), NSFTraceTags::MessageTag(), "Unknown Exception saving trace");
        }
    }
}
