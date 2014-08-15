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

#include "NSFTimerAction.h"

#include "NSFTimerThread.h"

namespace NorthStateFramework
{
    // Public

    NSFTimerAction::~NSFTimerAction()
    {
    }

    bool NSFTimerAction::isScheduled()
    {
        return NSFTimerThread::getPrimaryTimerThread().isScheduled(this);
    }

    void NSFTimerAction::schedule()
    {
        NSFTimerThread::getPrimaryTimerThread().scheduleAction(this, delayTime, repeatTime);
    }

    void NSFTimerAction::schedule(NSFTime delayTime)
    {
        NSFTimerThread::getPrimaryTimerThread().scheduleAction(this, delayTime, 0);
    }

    void NSFTimerAction::schedule(NSFTime delayTime, NSFTime repeatTime)
    {
        NSFTimerThread::getPrimaryTimerThread().scheduleAction(this, delayTime, repeatTime);
    }

    void NSFTimerAction::scheduleAbsoluteExecution()
    {
        NSFTimerThread::getPrimaryTimerThread().scheduleAction(this);
    }

    void NSFTimerAction::scheduleAbsoluteExecution(NSFTime executionTime)
    {
        setExecutionTime(executionTime);
        NSFTimerThread::getPrimaryTimerThread().scheduleAction(this);
    }

    void NSFTimerAction::scheduleAbsoluteExecution(NSFTime executionTime, NSFTime repeatTime)
    {
        setExecutionTime(executionTime);
        setRepeatTime(repeatTime);
        NSFTimerThread::getPrimaryTimerThread().scheduleAction(this);
    }

    void NSFTimerAction::unschedule()
    {
        NSFTimerThread::getPrimaryTimerThread().unscheduleAction(this);
    }

    // Protected

    NSFTimerAction::NSFTimerAction(const NSFString& name)
        : name(name.c_str()), delayTime(0), executionTime(0), repeatTime(0)
    {
    }
}
