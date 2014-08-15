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
