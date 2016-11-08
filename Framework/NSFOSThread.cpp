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

#include "NSFOSThread.h"
#include "NSFExceptionHandler.h"

namespace NorthStateFramework
{
    // Public

    NSFOSThread::~NSFOSThread()
    {
        delete action;
    }

    void NSFOSThread::executeAction()
    {
        try
        {
            (*action)(NSFContext(this));
        }
        catch(const std::exception& exception)
        {
            handleException(std::runtime_error(NSFString("Execution action exception: ") + exception.what()));
        }
        catch(...)
        {
            handleException(std::runtime_error(NSFString("Execution action exception: unknown exception")));
        }
    }

    // Protected

    NSFOSThread::NSFOSThread(const NSFString& name, const NSFVoidAction<NSFContext>& executionAction)
        : NSFTaggedObject(name), action(executionAction.copy())
    {
    }

    void NSFOSThread::handleException(const std::exception& exception)
    {
        std::runtime_error newException(getName() + " OS thread exception: " + exception.what());
        NSFExceptionHandler::getExceptionHandler().handleException(NSFExceptionContext(this, newException));
    }
}
