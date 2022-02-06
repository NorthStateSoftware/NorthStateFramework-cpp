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

#include "NSFEventHandler.h"

#include "NSFEnvironment.h"
#include "NSFEvent.h"
#include "NSFEventThread.h"
#include "NSFExceptionHandler.h"
#include "NSFTraceLog.h"

namespace NorthStateFramework
{
    int NSFEventHandler::terminationTimeout = 60000;

    // Public

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif

    NSFEventHandler::NSFEventHandler(const NSFString& name, NSFEventThread* thread)
        : NSFTaggedObject(name), loggingEnabled(true), eventThread(thread),
        eventHandlerMutex(NSFOSMutex::create()), runStatus(EventHandlerStopped), terminationStatus(EventHandlerReady), terminationSleepTime(10),
        startEvent("Start", this), stopEvent("Stop", this), terminateEvent("Terminate", this)
    {
        getEventThread()->addEventHandler(this);
    }

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( pop )
#endif

    NSFEventHandler::~NSFEventHandler()
    {
        terminate(true);
        delete eventHandlerMutex;
    }

    void NSFEventHandler::addEventReaction(NSFEvent* nsfEvent, const NSFVoidAction<NSFEventContext>& action)
    {
        LOCK(eventHandlerMutex)
        {
            // Check if event handler does not already exist for event
            if (eventReactions.find(nsfEvent->getId()) == eventReactions.end())
            {
                // The operator[] will create and return a new map value if one does not already exist
                eventReactions[nsfEvent->getId()].setExceptionAction(NSFAction(this, &NSFEventHandler::handleEventReactionException));
            }

            eventReactions[nsfEvent->getId()] += action;
        }
        ENDLOCK;
    }

    NSFEventStatus NSFEventHandler::handleEvent(NSFEvent* nsfEvent)
    {
        // Handle status changing events
        if ((nsfEvent == &startEvent))
        {
            runStatus = EventHandlerStarted;
        }
        else if (nsfEvent == &stopEvent)
        {
            runStatus = EventHandlerStopped;
        }
        else if (nsfEvent == &terminateEvent)
        {
            terminationStatus = EventHandlerTerminated;
            getEventThread()->removeEventHandler(this);
            return NSFEventHandled;
        }

        // Don't process events if stopped
        if (runStatus == EventHandlerStopped)
        {
            return NSFEventUnhandled;
        }

        // Process the event

        bool actionsToExecute = false;

        LOCK(eventHandlerMutex)
        {
            if (eventReactions.find(nsfEvent->getId()) != eventReactions.end())
            {
                actionsToExecute = true;
            }
        }
        ENDLOCK;

        if (actionsToExecute)
        {
            eventReactions[nsfEvent->getId()].execute(NSFEventContext(this, nsfEvent));
            return NSFEventHandled;
        }
        else
        {
            return NSFEventUnhandled;
        }
    }

    bool NSFEventHandler::hasEvent(NSFEvent* nsfEvent)
    {
        return eventThread->hasEvent(nsfEvent);
    }

    bool NSFEventHandler::hasEvent()
    {
        return eventThread->hasEventFor(this);
    }

    void NSFEventHandler::queueEvent(NSFEvent* nsfEvent)
    {
        LOCK(eventHandlerMutex)
        {
            // Do not allow events to be queued if terminating or terminated (i.e. not ready)
            if (terminationStatus != EventHandlerReady)
            {
                if (nsfEvent->getDeleteAfterHandling())
                {
                    delete nsfEvent;
                }

                return;
            }

            // Handle special case of terminate event by setting status and queuing a single terminate event.
            // Terminate event must be the last event queued to guarantee safe deletion when it is handled.
            if (nsfEvent == &terminateEvent)
            {
                if (terminationStatus == EventHandlerReady)
                {
                    terminationStatus = EventHandlerTerminating;
                }
            }

            nsfEvent->setDestination(this);
            eventThread->queueEvent(nsfEvent, false, getLoggingEnabled());
        }
        ENDLOCK;
    }

    void NSFEventHandler::queueEvent(NSFEvent* nsfEvent, INSFNamedObject* source)
    {
        nsfEvent->setSource(source);
        queueEvent(nsfEvent);
    }

    void NSFEventHandler::removeEventReaction(NSFEvent* nsfEvent, const NSFVoidAction<NSFEventContext>& action)
    {
        LOCK(eventHandlerMutex)
        {
            if (eventReactions.find(nsfEvent->getId()) != eventReactions.end())
            {
                eventReactions[nsfEvent->getId()] -= action;
            }
        }
        ENDLOCK;
    }

    void NSFEventHandler::startEventHandler()
    {
        queueEvent(&startEvent);
    }

    void NSFEventHandler::stopEventHandler()
    {
        queueEvent(&stopEvent);
    }

    void NSFEventHandler::terminate(bool waitForTerminated)
    {
        queueEvent(&terminateEvent);

        if (waitForTerminated)
        {
            for (int i = 0; i < terminationTimeout; i += terminationSleepTime)
            {
                if (terminationStatus == EventHandlerTerminated)
                {
                    return;
                }

                NSFOSThread::sleep(terminationSleepTime);
            }
            handleException(std::runtime_error(NSFString("Event handler was unable to terminate")));
        }
    }

    // Private

    void NSFEventHandler::handleEventReactionException(const NSFExceptionContext& context)
    {
        handleException(std::runtime_error(NSFString("Event reaction exception: ") + context.getException().what()));
    }

    void NSFEventHandler::handleException(const std::exception& exception)
    {
        // NSFExceptionContext maintains exception as a reference, so use two statements for proper scoping
        std::runtime_error newException(getName() + " event handler exception: " + exception.what());
        NSFExceptionContext newContext(this, newException);

        NSFExceptionHandler::getExceptionHandler().handleException(newContext);
    }
}
