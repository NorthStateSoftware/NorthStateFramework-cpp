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

#include "NSFTraceLog.h"

#include "NSFEventThread.h"
#include "NSFExceptionHandler.h"
#include "NSFTimerThread.h"

namespace NorthStateFramework
{
    // Public

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif

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

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( pop )
#endif

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
