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

#include "WorkHardPlayHard.h"

namespace WorkHardPlayHardExample
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    WorkHardPlayHard::WorkHardPlayHard(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)),

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        // Event constructors take the form (name, parent)
        breakEvent("Break", this),
        breakOverEvent("BreakOver", this),
        milestoneMetEvent("MilestoneMet", this),
        backToWorkEvent("BackToWork", this),

        // Regions and states, from outer to inner 
        // Initial state construtors take the form (name, parent)
        workHardPlayHardInitialState("WorkHardPlayHardInitial", this),
        // Composite state construtors take the form (name, parent, entry actions, exit actions)
        takeABreakState("TakeABreak", this, NULL, NULL),
        breakOverState("BreakOver", this, NULL, NULL),
        breakOverInitialState("BreakOverInitial", &breakOverState),
        breakOverHistoryState("BreakOverHistory", &breakOverState),
        workHardState("WorkHard", &breakOverState, NULL, NULL),
        playHardState("PlayHard", &breakOverState, NULL, NULL),

        // Transitions, ordered internal, local, external
        // External transition construtors take the form (name, source, target, trigger, guard, action)
        workHardPlayHardInitialToBreakOverTransition("WorkHardPlayHardInitialToBreakOver", &workHardPlayHardInitialState, &breakOverState, NULL, NULL, NULL),
        takeABreakToBreakOverTransition("TakeABreakToBreakOver", &takeABreakState, &breakOverState, &breakOverEvent, NULL, NULL),
        breakOverToTakeABreakTransition("BreakOverToTakeABreak", &breakOverState, &takeABreakState, &breakEvent, NULL, NULL),
        breakOverInitialToBreakOverHistoryTransition("BreakOverInitialToBreakOverHistory", &breakOverInitialState, &breakOverHistoryState, NULL, NULL, NULL),
        breakOverHistoryToWorkHardTransition("BreakOverHistoryToWorkHard", &breakOverHistoryState, &workHardState, NULL, NULL, NULL),
        workHardToPlayHardTransition("WorkHardToPlayHard", &workHardState, &playHardState, &milestoneMetEvent, NULL, NULL),
        playHardToWorkHardTransition("PlayHardToWorkHard", &playHardState, &workHardState, &backToWorkEvent, NULL, NULL)
    {
    }

    WorkHardPlayHard::~WorkHardPlayHard()
    {
        // It is good practice to stop event processing (terminate) in the 
        // destructor of a state machine or event handler to prevent 
        // entry/exit actions from being performed on a destructing object.
        terminate(true);
        delete getEventThread();
    }

    void WorkHardPlayHard::milestoneMet()
    {
        queueEvent(&milestoneMetEvent);
    }

    void WorkHardPlayHard::takeBreak()
    {
        queueEvent(&breakEvent);
    }

    void WorkHardPlayHard::breakOver()
    {
        queueEvent(&breakOverEvent);
    }

    void WorkHardPlayHard::backToWork()
    {
        queueEvent(&backToWorkEvent);
    }
}