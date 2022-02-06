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
