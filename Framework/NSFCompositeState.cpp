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
