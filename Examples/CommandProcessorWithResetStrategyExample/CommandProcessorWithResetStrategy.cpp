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

#include "CommandProcessorWithResetStrategy.h"

namespace CommandProcessorWithResetStrategyExample
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    CommandProcessorWithResetStrategy::CommandProcessorWithResetStrategy(const NSFString& name)
        : CommandProcessor(name), resetStrategy("ResetStrategy", &resetState)
    {
        resetToWaitForCommandTransition.Guards += NSFGuard(this, &CommandProcessorWithResetStrategy::isReady);
    }

    CommandProcessorWithResetStrategy::~CommandProcessorWithResetStrategy()
    {
        // It is good practice to stop event processing (terminate) in the 
        // destructor of a state machine or event handler to prevent 
        // entry/exit actions from being performed on a destructing object.
        terminate(true);
    }

    bool CommandProcessorWithResetStrategy::isReady(const NSFStateMachineContext&)
    {
        return resetStrategy.getReadyState().isActive();
    }
}