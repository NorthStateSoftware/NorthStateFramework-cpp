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

#include "NSFStateMachine.h"

#include "NSFEnvironment.h"
#include "NSFEventThread.h"
#include "NSFExceptionHandler.h"
#include "NSFTraceLog.h"
#include "NSFTransition.h"

namespace NorthStateFramework
{
    const NSFBoolGuard<NSFStateMachineContext>* NSFStateMachine::Else = NULL;
    int NSFStateMachine::terminationTimeout = 60000;

    // Public

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif

    NSFStateMachine::NSFStateMachine(const NSFString& name, NSFEventThread* thread)
        : NSFCompositeState(name, (NSFRegion*)NULL, NULL, NULL),
        consecutiveLoopCount(0), consecutiveLoopDetectionEnabled(true), consecutiveLoopLimit(1000), loggingEnabled(true),
        eventThread(thread),
        stateMachineMutex(NSFOSMutex::create()),
        runStatus(EventHandlerStopped), terminationStatus(EventHandlerReady), terminationSleepTime(10),
        resetEvent("Reset", this), runToCompletionEvent("RunToCompletion", this), startEvent("Start", this), stopEvent("Stop", this), terminateEvent("Terminate", this)
    {
        construct();
    }

    NSFStateMachine::NSFStateMachine(const NSFString& name, NSFRegion* parentRegion)
        : NSFCompositeState(name, parentRegion, NULL, NULL),
        consecutiveLoopCount(0), consecutiveLoopDetectionEnabled(true), consecutiveLoopLimit(1000), loggingEnabled(true),
        eventThread(getTopStateMachine()->getEventThread()),
        stateMachineMutex(NULL),
        runStatus(EventHandlerStopped), terminationStatus(EventHandlerReady), terminationSleepTime(10),
        resetEvent("Reset", this), runToCompletionEvent("RunToCompletion", this), startEvent("Start", this), stopEvent("Stop", this), terminateEvent("Terminate", this)
    {
        construct();
    }

    NSFStateMachine::NSFStateMachine(const NSFString& name, NSFCompositeState* parentState)
        : NSFCompositeState(name, parentState, NULL, NULL),
        consecutiveLoopCount(0), consecutiveLoopDetectionEnabled(true), consecutiveLoopLimit(1000), loggingEnabled(true),
        eventThread(getTopStateMachine()->getEventThread()),
        stateMachineMutex(NULL),
        runStatus(EventHandlerStopped), terminationStatus(EventHandlerReady), terminationSleepTime(10),
        resetEvent("Reset", this), runToCompletionEvent("RunToCompletion", this), startEvent("Start", this), stopEvent("Stop", this), terminateEvent("Terminate", this)
    {
        construct();
    }

#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( pop )
#endif

    NSFStateMachine::~NSFStateMachine()
    {
        terminate(true);
        delete stateMachineMutex;
    }

    NSFStateMachine* NSFStateMachine::getTopStateMachine()
    {
        if (isTopStateMachine())
        {
            return this;
        }
        else
        {
            return NSFCompositeState::getTopStateMachine();
        }
    }

    void NSFStateMachine::forceStateMachineEvaluation()
    {
        runToCompletion();
    }

    NSFEventStatus NSFStateMachine::handleEvent(NSFEvent* nsfEvent)
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
        else if (nsfEvent == &resetEvent)
        {
            reset();
        }

        // Don't process events if stopped
        if (runStatus == EventHandlerStopped)
        {
            return NSFEventUnhandled;
        }

        // If not already active, enter state machine at the root
        if (!active)
        {
            NSFStateMachineContext newContext (this, this, NULL, NULL, &startEvent);
            enter(newContext, false);
        }

        // Process the event
        NSFEventStatus eventStatus = NSFEventUnhandled;
        try
        {
            if (consecutiveLoopDetectionEnabled && (++consecutiveLoopCount >= consecutiveLoopLimit))
            {
                throw std::runtime_error("Consecutive loop limit exceeded");
            }

            eventStatus = processEvent(nsfEvent);

            if (eventStatus == NSFEventHandled)
            {
                runToCompletion();
            }
            else if (!eventThread->hasEventFor(this))
            {
                // If no more events for this state machine and last event was unhandled,
                // then the state machine has paused, indicating it's not in an infinite loop.
                consecutiveLoopCount = 0;
            }
        }
        catch(const std::exception& exception)
        {
            handleException(std::runtime_error(nsfEvent->getName() + " event handling exception: " + exception.what()));
        }
        catch(...)
        {
            handleException(std::runtime_error(nsfEvent->getName() + " event handling exception: unknown exception"));
        }

        return eventStatus;
    }

    void NSFStateMachine::queueEvent(NSFEvent* nsfEvent)
    {
        queueEvent(nsfEvent, false, getLoggingEnabled());
    }

    void NSFStateMachine::queueEvent(NSFEvent* nsfEvent, INSFNamedObject* source)
    {
        nsfEvent->setSource(source);
        queueEvent(nsfEvent);
    }

    void NSFStateMachine::resetStateMachine()
    {
        queueEvent(&resetEvent);
    }

    void NSFStateMachine::startEventHandler()
    {
        startStateMachine();
    }

    void NSFStateMachine::stopEventHandler()
    {
        stopStateMachine();
    }

    void NSFStateMachine::startStateMachine()
    {
        queueEvent(&startEvent);
    }

    void NSFStateMachine::stopStateMachine()
    {
        queueEvent(&stopEvent);
    }

    void NSFStateMachine::terminate(bool waitForTerminated)
    {
        if (!isTopStateMachine())
        {
            getTopStateMachine()->terminate(waitForTerminated);
            return;
        }

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
            handleException(std::runtime_error(NSFString("State machine was unable to terminate")));
        }
    }

    // Protected

    void NSFStateMachine::handleException(const std::exception& exception)
    {
        // NSFExceptionContext maintains exception as a reference, so use two statements for proper scoping
        std::runtime_error newException(getName() + " state machine exception: " + exception.what());
        NSFExceptionContext newContext(this, newException);

        ExceptionActions.execute(newContext);
        NSFExceptionHandler::getExceptionHandler().handleException(newContext);
    }

    bool NSFStateMachine::hasEvent(NSFEvent* nsfEvent)
    {
        return eventThread->hasEvent(nsfEvent);
    }

    bool NSFStateMachine::hasEvent()
    {
        return eventThread->hasEventFor(this);
    }

    bool NSFStateMachine::isTopStateMachine()
    {
        return (parentRegion == NULL);
    }

    void NSFStateMachine::rerouteTransition(NSFTransition* transition, NSFState* source, NSFState* target)
    {
        transition->getSource()->removeOutgoingTransition(transition);
        transition->getTarget()->removeIncomingTransition(transition);
        transition->setSource(source);
        transition->setTarget(target);
    }

    void NSFStateMachine::reset()
    {
        // Base class behavior
        NSFCompositeState::reset();

        // Additional behavior
        consecutiveLoopCount = 0;
    }

    // Private

    void NSFStateMachine::construct()
    {
        StateChangeActions.setExceptionAction(NSFAction(this, &NSFStateMachine::handleStateChangeActionException));

        if (isTopStateMachine())
        {
            getEventThread()->addEventHandler(this);
        }
    }

    void NSFStateMachine::executeStateChangeActions(NSFStateMachineContext& context)
    {
        StateChangeActions.execute(context);

        NSFStateMachine* parentStateMachine = getParentStateMachine();
        if (parentStateMachine != NULL)
        {
            parentStateMachine->executeStateChangeActions(context);
        }
    }

    void NSFStateMachine::handleStateChangeActionException(const NSFExceptionContext& context)
    {
        handleException(std::runtime_error(NSFString("State change action exception: ") + context.getException().what()));
    }

    void NSFStateMachine::queueEvent(NSFEvent* nsfEvent, bool isPriorityEvent, bool logEventQueued)
    {
        if (!isTopStateMachine())
        {
            getTopStateMachine()->queueEvent(nsfEvent, isPriorityEvent, logEventQueued);
            return;
        }

        LOCK(stateMachineMutex)
        {
            // Do not allow events to be queued if terminating or terminated,
            // except for run to completion event, which may be queued if terminating to allow proper semantics to continue until terminated.
            if ((terminationStatus == EventHandlerTerminated) ||
                ((terminationStatus == EventHandlerTerminating) && (nsfEvent != &runToCompletionEvent)))
            {
                if (nsfEvent->getDeleteAfterHandling())
                {
                    delete nsfEvent;
                }

                return;
            }

            // Handle special case of terminate event by setting status and queuing a single terminate event.
            // Terminate event must be the last event queued to guarantee safe deletion after it is handled.
            if (nsfEvent == &terminateEvent)
            {
                if (terminationStatus == EventHandlerReady)
                {
                    terminationStatus = EventHandlerTerminating;
                }
            }

            nsfEvent->setDestination(this);

            eventThread->queueEvent(nsfEvent, isPriorityEvent, logEventQueued);
        }
        ENDLOCK;
    }

    void NSFStateMachine::runToCompletion()
    {
        queueEvent(&runToCompletionEvent, true, false);
    }
}
