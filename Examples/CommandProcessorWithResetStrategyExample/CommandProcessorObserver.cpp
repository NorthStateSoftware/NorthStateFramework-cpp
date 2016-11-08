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

#include "CommandProcessorObserver.h"

namespace CommandProcessorWithResetStrategyExample
{
    CommandProcessorObserver::CommandProcessorObserver(CommandProcessor& commandProcessor)
    {
        // Register handleStateChange to be called on state entry.
        commandProcessor.getWaitForCommandState().EntryActions += NSFAction(this, &CommandProcessorObserver::handleStateEntered);
        commandProcessor.getWaitForResponseState().EntryActions += NSFAction(this, &CommandProcessorObserver::handleStateEntered);
        commandProcessor.getErrorState().EntryActions += NSFAction(this, &CommandProcessorObserver::handleStateEntered);
        commandProcessor.getResetState().EntryActions += NSFAction(this, &CommandProcessorObserver::handleStateEntered);

        // Register handleStateChange to be called on state exit.
        commandProcessor.getWaitForCommandState().ExitActions += NSFAction(this, &CommandProcessorObserver::handleStateExited);
        commandProcessor.getWaitForResponseState().ExitActions += NSFAction(this, &CommandProcessorObserver::handleStateExited);
        commandProcessor.getErrorState().ExitActions += NSFAction(this, &CommandProcessorObserver::handleStateExited);
        commandProcessor.getResetState().ExitActions += NSFAction(this, &CommandProcessorObserver::handleStateExited);

        // Register transition actions to be called on transition from one state to another.
        commandProcessor.getWaitForCommandToWaitForResponseTransition().Actions += NSFAction(this, &CommandProcessorObserver::handleTransition);
        commandProcessor.getWaitForResponseToWaitForCommandTransition().Actions += NSFAction(this, &CommandProcessorObserver::handleTransition);
        commandProcessor.getWaitForResponseToErrorTransition().Actions += NSFAction(this, &CommandProcessorObserver::handleTransition);
        commandProcessor.getErrorToResetTransition().Actions += NSFAction(this, &CommandProcessorObserver::handleTransition);
        commandProcessor.getResetToWaitForCommandTransition().Actions += NSFAction(this, &CommandProcessorObserver::handleTransition);
    }

    void CommandProcessorObserver::handleStateEntered(const NSFStateMachineContext& context)
    {
        NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Entering ") + context.getEnteringState()->getName());
    }

    void CommandProcessorObserver::handleTransition(const NSFStateMachineContext& context)
    {
        NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Transitioning from ") + context.getTransition()->getSource()->getName() +
            " to " + context.getTransition()->getTarget()->getName());
    }

    void CommandProcessorObserver::handleStateExited(const NSFStateMachineContext& context)
    {
        NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Exiting ") + context.getExitingState()->getName());
    }
}
