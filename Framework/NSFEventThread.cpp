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

#include "NSFEventThread.h"

#include "NSFEvent.h"
#include "NSFEventHandler.h"
#include "NSFExceptionHandler.h"
#include "NSFTimerThread.h"
#include "NSFTraceLog.h"

namespace NorthStateFramework
{
    // Public

    NSFEventThread::NSFEventThread(const NSFString& name)
        : NSFThread(name), signal(NSFOSSignal::create(name))
    {
        startThread();
    }

    NSFEventThread::NSFEventThread(const NSFString& name, int priority)
        : NSFThread(name, priority), signal(NSFOSSignal::create(name))
    {
        startThread();
    }

    NSFEventThread::~NSFEventThread()
    {
        terminate(true);
        clearEvents();
        delete signal;
    }

    std::list<INSFEventHandler*> NSFEventThread::getEventHandlers()
    {
        LOCK(getThreadMutex())
        {
            return eventHandlers;
        }
        ENDLOCK;
    }

    bool NSFEventThread::hasEvent(NSFEvent* nsfEvent)
    {
        LOCK(getThreadMutex())
        {
            std::list<NSFEvent*>::iterator eventIterator;
            for (eventIterator = nsfEvents.begin(); eventIterator != nsfEvents.end(); ++eventIterator)
            {
                if ((*eventIterator)->getId() == nsfEvent->getId())
                {
                    return true;
                }
            }
            return false;
        }
        ENDLOCK;
    }

    bool NSFEventThread::hasEventFor(INSFEventHandler* eventHandler)
    {
        LOCK(getThreadMutex())
        {
            std::list<NSFEvent*>::iterator eventIterator;
            for (eventIterator = nsfEvents.begin(); eventIterator != nsfEvents.end(); ++eventIterator)
            {
                if ((*eventIterator)->getDestination() == eventHandler)
                {
                    return true;
                }
            }
            return false;
        }
        ENDLOCK;
    }

    void NSFEventThread::queueEvent(NSFEvent* nsfEvent, bool isPriorityEvent, bool logEventQueued)
    {
        // Do not allow events to be queued if terminated
        if (getTerminationStatus() == ThreadTerminated)
        {
            if (nsfEvent->getDeleteAfterHandling())
            {
                delete nsfEvent;
            }

            return;
        }

        LOCK(getThreadMutex())
        {
            if (logEventQueued)
            {
                if (nsfEvent->getSource() != NULL)
                {
                    NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::EventQueuedTag(),
                        NSFTraceTags::NameTag(), nsfEvent->getName(),
                        NSFTraceTags::SourceTag(), nsfEvent->getSource()->getName(),
                        NSFTraceTags::DestinationTag(), nsfEvent->getDestination()->getName());
                }
                else
                {
                    NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::EventQueuedTag(),
                        NSFTraceTags::NameTag(), nsfEvent->getName(),
                        NSFTraceTags::SourceTag(), NSFTraceTags::UnknownTag(),
                        NSFTraceTags::DestinationTag(), nsfEvent->getDestination()->getName());
                }
            }

            if (isPriorityEvent)
            {
                nsfEvents.push_front(nsfEvent);
            }
            else
            {
                nsfEvents.push_back(nsfEvent);
            }
        }
        ENDLOCK;

        signal->send();
    }

    void NSFEventThread::terminate(bool waitForTerminated)
    {
        // Get all event handler terminations started
        std::list<INSFEventHandler*>::iterator eventHandlerIterator;
        std::list<INSFEventHandler*> eventHandlersCopy = getEventHandlers();
        for (eventHandlerIterator = eventHandlersCopy.begin(); eventHandlerIterator != eventHandlersCopy.end(); ++eventHandlerIterator)
        {
            (*eventHandlerIterator)->terminate(false);
        }

        // Base class behavior, but return immediately so signal can be sent to wake up event thread
        NSFThread::terminate(false);

        signal->send();

        // Wait as specified for thread to terminate after signal has been sent
        NSFThread::terminate(waitForTerminated);
    }

    // Protected

    // Private

    bool NSFEventThread::allEventHandlersTerminated()
    {
        std::list<INSFEventHandler*>::iterator eventHandlerIterator;
        std::list<INSFEventHandler*> eventHandlersCopy = getEventHandlers();
        for (eventHandlerIterator = eventHandlersCopy.begin(); eventHandlerIterator != eventHandlersCopy.end(); ++eventHandlerIterator)
        {
            if ((*eventHandlerIterator)->getTerminationStatus() != EventHandlerTerminated)
            {
                return false;
            }
        }

        return true;
    }

    void NSFEventThread::addEventHandler(INSFEventHandler* eventHandler)
    {
        LOCK(getThreadMutex())
        {
            if (getTerminationStatus() == ThreadReady)
            {
                eventHandlers.push_back(eventHandler);
            }
        }
        ENDLOCK;
    }

    void NSFEventThread::clearEvents()
    {
        LOCK(getThreadMutex())
        {
            while (!nsfEvents.empty())
            {
                NSFEvent* nsfEvent = nsfEvents.front();
                nsfEvents.pop_front();

                if (nsfEvent->getDeleteAfterHandling())
                {
                    delete nsfEvent;
                }
            }
        }
        ENDLOCK;
    }

    void NSFEventThread::removeEventHandler(INSFEventHandler* eventHandler)
    {
        LOCK(getThreadMutex())
        {
            eventHandlers.remove(eventHandler);
        }
        ENDLOCK;
    }

    void NSFEventThread::threadLoop()
    {
        while (true)
        {
            // Wait for signal to indicate there's work to do
            signal->wait();

            while (true)
            {
                NSFEvent* nsfEvent = NULL;

                LOCK(getThreadMutex())
                {
                    if (!nsfEvents.empty())
                    {
                        nsfEvent = nsfEvents.front();
                        nsfEvents.pop_front();
                    }
                }
                ENDLOCK;

                if (nsfEvent == NULL)
                {
                    break;
                }

                // Guard a bad event from taking down event thread
                try
                {
                    nsfEvent->getDestination()->handleEvent(nsfEvent);
                }
                catch(const std::exception& exception)
                {
                    handleException(std::runtime_error(getName() + " event handling exception: " + exception.what()));
                }
                catch(...)
                {
                    handleException(std::runtime_error(getName() + " event handling exception: unknown exception"));
                }

                if (nsfEvent->getDeleteAfterHandling())
                {
                    delete nsfEvent;
                }
            }

            // Thread loop will exit when terminating and all event handlers are terminated
            if ((getTerminationStatus() ==  ThreadTerminating) && allEventHandlersTerminated())
            {
                clearEvents();
                return;
            }
        }
    }
}
