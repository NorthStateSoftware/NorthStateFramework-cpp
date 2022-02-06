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

#include "NSFDebugUtility.h"

#include "NSFEventThread.h"
#include <iostream>


namespace NorthStateFramework
{
    // Public

    int NSFDebugUtility::getMaxPendingWrites()
    {
        LOCK(debugUtilityMutex)
        {
            return maxPendingWrites;
        }
        
        ENDLOCK;
    }

    void NSFDebugUtility::setMaxPendingWrites(int value)
    {
        LOCK(debugUtilityMutex)
        {
            maxPendingWrites = value;
        }
        ENDLOCK;
    }

    bool NSFDebugUtility::writeToConsole(const NSFString& text)
    {
        LOCK(debugUtilityMutex)
        {
            if (pendingWriteCount >= maxPendingWrites)
            {
                return false;
            }

            ++pendingWriteCount;

            eventHandler.queueEvent(writeToConsoleEvent.copy(true, text));
        }
        ENDLOCK;

        return true;
    }

    bool NSFDebugUtility::writeLineToConsole(const NSFString& text)
    {
        LOCK(debugUtilityMutex)
        {
            if (pendingWriteCount >= maxPendingWrites)
            {
                return false;
            }

            ++pendingWriteCount;

            eventHandler.queueEvent(writeLineToConsoleEvent.copy(true, text));
        }
        ENDLOCK;

        return true;
    }

    bool NSFDebugUtility::allOutputTextWritten()
    {
        return !(eventHandler.hasEvent());
    }

    // Private

    NSFDebugUtility& NSFDebugUtility::debugUtility(getPrimaryDebugUtility());


    NSFDebugUtility::NSFDebugUtility()
        : NSFTaggedObject("DebugUtility"), eventHandler(getName(), new NSFEventThread(getName(), NSFOSThread::getLowestPriority())),
        writeToConsoleEvent("WriteToConsole", &eventHandler), writeLineToConsoleEvent("WriteLineToConsole", &eventHandler), 
        debugUtilityMutex(NSFOSMutex::create()), pendingWriteCount(0), maxPendingWrites(100), lastWriteOnSeparateLine(true)
    {
        eventHandler.setLoggingEnabled(false);
        eventHandler.addEventReaction(&writeToConsoleEvent, NSFAction(this, &NSFDebugUtility::performWriteToConsole));
        eventHandler.addEventReaction(&writeLineToConsoleEvent, NSFAction(this, &NSFDebugUtility::performWriteLineToConsole));
        eventHandler.startEventHandler();
    }

    NSFDebugUtility::~NSFDebugUtility()
    {
        eventHandler.terminate(true);
        delete eventHandler.getEventThread();
    }

    void NSFDebugUtility::performWriteToConsole(const NSFEventContext& context)
    {
        std::cout << ((NSFDataEvent<NSFString>*)context.getEvent())->getData();

        lastWriteOnSeparateLine = false;

        LOCK(debugUtilityMutex)
        {
            --pendingWriteCount;
        }
        ENDLOCK;
    }

    void NSFDebugUtility::performWriteLineToConsole(const NSFEventContext& context)
    {
        if (!lastWriteOnSeparateLine)
        {
            std::cout << std::endl;
        }

        std::cout << ((NSFDataEvent<NSFString>*)context.getEvent())->getData() << std::endl;

        lastWriteOnSeparateLine = true;

        LOCK(debugUtilityMutex)
        {
            --pendingWriteCount;
        }
        ENDLOCK;
    }
}
