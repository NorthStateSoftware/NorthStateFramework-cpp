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

#include "NSFCompositeState.h"

#include "NSFRegion.h"
#include "NSFStateMachine.h"

namespace NorthStateFramework
{
    // Public

    NSFCompositeState::NSFCompositeState(const NSFString& name, NSFRegion* parentRegion, const NSFVoidAction<NSFStateMachineContext>& entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction)
        : NSFState(name, parentRegion, entryAction, exitAction), defaultRegion(NULL)
    {
    }

    NSFCompositeState::NSFCompositeState(const NSFString& name, NSFCompositeState* parentState, const NSFVoidAction<NSFStateMachineContext>& entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction)
        : NSFState(name, parentState, entryAction, exitAction), defaultRegion(NULL)
    {
    }

    NSFCompositeState::NSFCompositeState(const NSFString& name, NSFRegion* parentRegion, NSFVoidAction<NSFStateMachineContext>* entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction)
        : NSFState(name, parentRegion, entryAction, exitAction), defaultRegion(NULL)
    {
    }

    NSFCompositeState::NSFCompositeState(const NSFString& name, NSFCompositeState* parentState, NSFVoidAction<NSFStateMachineContext>* entryAction, const NSFVoidAction<NSFStateMachineContext>& exitAction)
        : NSFState(name, parentState, entryAction, exitAction), defaultRegion(NULL)
    {
    }

    NSFCompositeState::NSFCompositeState(const NSFString& name, NSFRegion* parentRegion, const NSFVoidAction<NSFStateMachineContext>& entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction)
        : NSFState(name, parentRegion, entryAction, exitAction), defaultRegion(NULL)
    {
    }

    NSFCompositeState::NSFCompositeState(const NSFString& name, NSFCompositeState* parentState, const NSFVoidAction<NSFStateMachineContext>& entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction)
        : NSFState(name, parentState, entryAction, exitAction), defaultRegion(NULL)
    {
    }

    NSFCompositeState::NSFCompositeState(const NSFString& name, NSFRegion* parentRegion, NSFVoidAction<NSFStateMachineContext>* entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction)
        : NSFState(name, parentRegion, entryAction, exitAction), defaultRegion(NULL)
    {
    }

    NSFCompositeState::NSFCompositeState(const NSFString& name, NSFCompositeState* parentState, NSFVoidAction<NSFStateMachineContext>* entryAction, NSFVoidAction<NSFStateMachineContext>* exitAction)
        : NSFState(name, parentState, entryAction, exitAction), defaultRegion(NULL)
    {
    }

    NSFCompositeState::~NSFCompositeState()
    {
        delete defaultRegion;
    }

    bool NSFCompositeState::isInState(NSFState* state)
    {
        if (!isActive())
        {
            return false;
        }

        // Base class behavior
        if (NSFState::isInState(state))
        {
            return true;
        }

        // Check regions
        std::list<NSFRegion*>::iterator regionIterator;
        for (regionIterator = regions.begin(); regionIterator != regions.end(); ++regionIterator)
        {
            if ((*regionIterator)->isInState(state))
            {
                return true;
            }
        }

        return false;
    }

    bool NSFCompositeState::isInState(const NSFString& stateName)
    {
        if (!isActive())
        {
            return false;
        }

        // Base class behavior
        if (NSFState::isInState(stateName))
        {
            return true;
        }

        // Check regions
        std::list<NSFRegion*>::iterator regionIterator;
        for (regionIterator = regions.begin(); regionIterator != regions.end(); ++regionIterator)
        {
            if ((*regionIterator)->isInState(stateName))
            {
                return true;
            }
        }

        return false;
    }

    void NSFCompositeState::enter(NSFStateMachineContext& context, bool useHistory)
    {
        // Base class behavior
        NSFState::enter(context, useHistory);

        // Additional behavior
        enterRegions(context, useHistory);
    }

    void NSFCompositeState::exit(NSFStateMachineContext& context)
    {
        // Additional behavior
        exitRegions(context);

        // Base class behavior
        NSFState::exit(context);
    }

    NSFEventStatus NSFCompositeState::processEvent(NSFEvent* nsfEvent)
    {
        // Additional behavior

        // Let regions process event first
        NSFEventStatus eventStatus = NSFEventUnhandled;

        std::list<NSFRegion*>::iterator regionIterator;
        for (regionIterator = regions.begin(); regionIterator != regions.end(); ++regionIterator)
        {
            NSFEventStatus status = (*regionIterator)->processEvent(nsfEvent);

            if (status == NSFEventHandled)
            {
                eventStatus = NSFEventHandled;
            }
        }

        if (eventStatus == NSFEventHandled)
        {
            return NSFEventHandled;
        }

        // Base class behavior
        return NSFState::processEvent(nsfEvent);
    }

    void NSFCompositeState::reset()
    {
        // Base class behavior
        NSFState::reset();

        // Reset regions
        std::list<NSFRegion*>::iterator regionIterator;
        for (regionIterator = regions.begin(); regionIterator != regions.end(); ++regionIterator)
        {
            (*regionIterator)->reset();
        }
    }

    // Private

    void NSFCompositeState::addRegion(NSFRegion* region)
    {
        // Add region if not already in list
        if (std::find(regions.begin(), regions.end(), region) == regions.end())
        {
            regions.push_back(region);
        }
    }

    void NSFCompositeState::enterRegions(NSFStateMachineContext& context, bool useHistory)
    {
        std::list<NSFRegion*>::iterator regionIterator;
        for (regionIterator = regions.begin(); regionIterator != regions.end(); ++regionIterator)
        {
            if (!(*regionIterator)->isActive())
            {
                (*regionIterator)->enter(context, useHistory);
            }
        }
    }

    void NSFCompositeState::exitRegions(NSFStateMachineContext& context)
    {
        std::list<NSFRegion*>::iterator regionIterator;
        for (regionIterator = regions.begin(); regionIterator != regions.end(); ++regionIterator)
        {
            if ((*regionIterator)->isActive())
            {
                (*regionIterator)->exit(context);
            }
        }
    }

    NSFRegion* NSFCompositeState::getDefaultRegion()
    {
        if (defaultRegion == NULL)
        {
            defaultRegion = new NSFRegion(getName() + "DefaultRegion", this);
        }
        return defaultRegion;
    }
}
