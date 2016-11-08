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

#ifndef NSF_TRACE_LOG_H
#define NSF_TRACE_LOG_H

#include "NSFDataEvent.h"
#include "NSFEventHandler.h"
#include "NSFTaggedTypes.h"
#include "NSFXMLDocument.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents commonly used tags for the trace log.
    /// </summary>
    class NSFTraceTags
    {
    public:

        static const NSFString& ActionExecutedTag() { static NSFString string("ActionExecuted"); return string; };
        static const NSFString& ActionTag() { static NSFString string("Action"); return string; };
        static const NSFString& DestinationTag() { static NSFString string("Destination"); return string; };
        static const NSFString& ErrorTag() { static NSFString string("Error"); return string; };
        static const NSFString& EventQueuedTag() { static NSFString string("EventQueued"); return string; };
        static const NSFString& EventTag() { static NSFString string("Event"); return string; };
        static const NSFString& ExceptionTag() { static NSFString string("Exception"); return string; };
        static const NSFString& InformationalTag() { static NSFString string("Informational"); return string; };
        static const NSFString& MessageReceivedTag() { static NSFString string("MessageReceived"); return string; };
        static const NSFString& MessageSentTag() { static NSFString string("MessageSent"); return string; };
        static const NSFString& MessageTag() { static NSFString string("Message"); return string; };
        static const NSFString& NameTag() { static NSFString string("Name"); return string; };
        static const NSFString& ObjectTag() { static NSFString string("Object"); return string; };
        static const NSFString& SourceTag() { static NSFString string("Source"); return string; };
        static const NSFString& StateEnteredTag() { static NSFString string("StateEntered"); return string; };
        static const NSFString& StateMachineTag() { static NSFString string("StateMachine"); return string; };
        static const NSFString& StateTag() { static NSFString string("State"); return string; };
        static const NSFString& UnknownTag() { static NSFString string("Unknown"); return string; };
        static const NSFString& ValueTag() { static NSFString string("Value"); return string; };
        static const NSFString& VariableTag() { static NSFString string("Variable"); return string; };
    };

    /// <summary>
    /// Represents a trace log.
    /// </summary>
    class NSFTraceLog : public NSFTaggedObject
    {
    public:

        /// <summary>
        /// Creates a trace log
        /// </summary>
        /// <param name="name">The name of the log.</param>
        /// <remarks>
        /// By default, thread priority is set to lowest.
        /// </remarks>
        NSFTraceLog(const NSFString& name);

        /// <summary>
        /// Destroys a trace log
        /// </summary>
        virtual ~NSFTraceLog();

        /// <summary>
        /// Gets the framework trace log.
        /// </summary>
        /// <remarks>
        /// This log is where framework traces are recorded, and where static logging operations record traces.
        /// Most application use this log for custom trace recording, as well.
        /// </remarks>
        static NSFTraceLog& getPrimaryTraceLog() { static NSFTraceLog traceLog = NSFTraceLog("PrimaryTraceLog"); return traceLog; };

        /// <summary>
        /// Gets the flag indicating if tracing is enabled.
        /// </summary>
        /// <remarks>
        /// If tracing is disabled, no additions will be made to the trace log.
        /// </remarks>
        bool getEnabled() const { return enabled; }

        /// <summary>
        /// Sets the flag indicating if tracing is enabled.
        /// </summary>
        /// <remarks>
        /// If tracing is disabled, no additions will be made to the trace log.
        /// </remarks>
        void setEnabled(bool value) { enabled = value; }

        /// <summary>
        /// Gets the maximum number of trace entries in the log.
        /// </summary>
        /// <remarks>
        /// Once the trace limit is reached, old traces are deleted in favor of new ones.
        /// </remarks>
        UInt32 getMaxTraces() const { return maxTraces; }

        /// <summary>
        /// Sets the maximum number of trace entries in the log.
        /// </summary>
        /// <remarks>
        /// Once the trace limit is reached, old traces are deleted in favor of new ones.
        /// </remarks>
        void  setMaxTraces(UInt32 value) { maxTraces = value; }

        /// <summary>
        /// Adds a trace to the log.
        /// </summary>
        /// <param name="type">The type of the trace.</param>
        /// <param name="tag">The tag associated with the trace.</param>
        /// <param name="data">The data associated with the tag.</param>
        void addTrace(const NSFString& type, const NSFString& tag, const NSFString& data);

        /// <summary>
        /// Adds a trace to the log.
        /// </summary>
        /// <param name="type">The type of the trace.</param>
        /// <param name="tag1">The first tag associated with the trace.</param>
        /// <param name="data1">The data associated with the first tag.</param>
        /// <param name="tag2">The second tag associated with the trace.</param>
        /// <param name="data2">The data associated with the second tag.</param>
        void addTrace(const NSFString& type, const NSFString& tag1, const NSFString& data1, const NSFString& tag2, const NSFString& data2);

        /// <summary>
        /// Adds a trace to the log.
        /// </summary>
        /// <param name="type">The type of the trace.</param>
        /// <param name="tag1">The first tag associated with the trace.</param>
        /// <param name="data1">The data associated with the first tag.</param>
        /// <param name="tag2">The second tag associated with the trace.</param>
        /// <param name="data2">The data associated with the second tag.</param>
        /// <param name="tag3">The third tag associated with the trace.</param>
        /// <param name="data3">The data associated with the third tag.</param>
        void addTrace(const NSFString& type, const NSFString& tag1, const NSFString& data1, const NSFString& tag2, const NSFString& data2, const NSFString& tag3, const NSFString& data3);

        /// <summary>
        /// Have all traces that have been added been logged
        /// </summary>
        /// <returns>
        /// True if the NSFTraceLog has logged all the added traces.
        /// False if there are traces that have yet to be processed.
        /// </returns>
        bool allTracesLogged();

        /// <summary>
        /// Saves the trace log to a file with the name of the log.
        /// </summary>
        /// <remarks>
        /// The trace log is saved in xml format.
        /// </remarks>
        void saveLog();

        /// <summary>
        /// Saves the trace log to the specified filename.
        /// </summary>
        /// <param name="fileName">The relative or fully qualified name for the file.</param>
        /// <remarks>
        /// The trace log is saved in xml format.
        /// </remarks>
        void saveLog(const NSFString& fileName);

    protected:

        /// <summary>
        /// Reaction that does the work adding a trace element to the log.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void addTraceToLog(const NSFEventContext& context);

        /// <summary>
        /// Reaction that does the work of saving the file.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void saveTrace(const NSFEventContext& context);

    private:

        bool enabled;
        NSFEventHandler eventHandler;
        UInt32 maxTraces;
        NSFDataEvent<NSFXMLElement*> traceAddEvent;
        NSFOSMutex* traceLogMutex;
        NSFDataEvent<NSFString> traceSaveEvent;
        NSFXMLDocument xmlLog;

        static const NSFString& TimeTag() { static NSFString string("Time"); return string; };
        static const NSFString& TraceTag() { static NSFString string("Trace"); return string; };
        static const NSFString& TraceLogTag() { static NSFString string("TraceLog"); return string; };
        static const NSFString& TraceAddString() { static NSFString string("TraceAdd"); return string; };
        static const NSFString& TraceSaveString() { static NSFString string("TraceSave"); return string; };
        static const NSFString& TraceSaveCompleteString() { static NSFString string("TraceSaveComplete"); return string; };
    };
}

#endif // NSF_TRACE_LOG_H
