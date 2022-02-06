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
