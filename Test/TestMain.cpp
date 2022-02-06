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

#include "TestMain.h"
#include "NorthStateFramework.h"

#include <iostream>
#if defined WIN32
#include <windows.h>
#endif

using namespace NSFTest;

void globalHandleException(const NSFExceptionContext& context)
{
    if (NSFString(context.getException().what()).find(ExceptionHandlingTest::IntentionalExceptionString()) == NSFString::npos)
    {
        NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Exception caught: ") + context.getException().what());
    }
}

int main(int argc, char* argv[]) 
{ 
    // If NSF_AUTO_START is defined in NSFCustomConfig.h (default behavior), then calling start is not necessary.
    // NSFEnvironment::getEnvironment().start();

#if defined WIN32
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    //SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

    NSFTraceLog::getPrimaryTraceLog().setEnabled(true);

    std::list<ITestInterface*> tests;
    string continuousString = "/c";
    if ((argc >= 2) && (continuousString == argv[1]))
    {
        tests.push_back(new ContinuouslyRunningTest("Continuously Running Test", 100));
    }
    else
    {
        tests.push_back(new TimerResolutionTest("Timer Resolution Test"));
        tests.push_back(new TimerAccuracyTest("Timer Accuracy Test"));
        tests.push_back(new TimerGetTimeTest("Timer Get Time Test"));
        tests.push_back(new ContextSwitchTest("Context Switch Test"));
        tests.push_back(new TrivialStateMachineTest("Trivial State Machine Test"));
        tests.push_back(new BasicStateMachineTest("Basic State Machine Test"));
        tests.push_back(new ShallowHistoryTest("Shallow History Test"));
        tests.push_back(new DeepHistoryTest("Deep History Test"));
        tests.push_back(new BasicForkJoinTest("Basic Fork Join Test"));
        tests.push_back(new DeepHistoryReEntryTest("Deep History Re-entry Test"));
        tests.push_back(new StateMachineRestartTest("State Machine Restart Test"));
        tests.push_back(new ForkJoinToForkJoinTransitionTest("Fork Join to Fork Join Transition Test"));
        tests.push_back(new MultipleTriggersOnTransitionTest("Multiple Triggers on Transition Test"));
        tests.push_back(new ExtendedRunTest("Extended Run Test")); 
        tests.push_back(new ExceptionHandlingTest("Exception Handling Test"));
        tests.push_back(new StrategyTest("Strategy Test"));
        tests.push_back(new TransitionOrderTest("Transition Order Test"));
        tests.push_back(new ChoiceStateTest("Choice State Test"));
        tests.push_back(new DocumentNavigationTest("Document Test Load", "ValidTest.xml"));
        tests.push_back(new DocumentLoadTest("Invalid Document Test Load", "InvalidTest.xml"));
        tests.push_back(new ThreadCreationTest("Thread Creation Test"));
        tests.push_back(new StateMachineDeleteTest("State Machine Delete Test"));
        tests.push_back(new TraceAddTest("Trace Add Test", 10000));
        tests.push_back(new TimerObservedTimeGapTest("Timer Observed Time Gap Test"));
        tests.push_back(new MultipleStateMachineStressTest("MultipleStateMachineStressTest", 100, 100));
        tests.push_back(new TimerObservedTimeGapTest("Timer Observed Time Gap Test"));
    }

    NSFExceptionHandler::getExceptionHandler().ExceptionActions += NSFAction(&globalHandleException);

    bool allTestPassed = true;

    std::list<ITestInterface*>::const_iterator testIterator;
    for (testIterator = tests.begin(); testIterator != tests.end(); ++testIterator)
    {
        NSFString errorMessage;
        if ((*testIterator)->runTest(errorMessage))
        {
            NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Passed - ") + (*testIterator)->getName());
        }
        else
        {
            allTestPassed = false;
            NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Failed - ") + (*testIterator)->getName());
            NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString(errorMessage));
            NSFTraceLog::getPrimaryTraceLog().saveLog("TraceFile_" + (*testIterator)->getName() + "_Failed.xml");
        }
    }

    if (allTestPassed)
    {
        NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("All tests passed"));
        NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::InformationalTag(), "Test", "End");
        NSFTraceLog::getPrimaryTraceLog().saveLog("TraceFile_AllPassed.xml");
    }
    else
    {
        NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Some test failed"));
        NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::InformationalTag(), "Test", "End");
        NSFTraceLog::getPrimaryTraceLog().saveLog("TraceFile_SomeFailed.xml");
    }

    NSFDebugUtility::getPrimaryDebugUtility().writeLineToConsole(NSFString("Press Enter key to continue"));
    cin.get();

    while (!tests.empty())
    {
        delete tests.front();
        tests.pop_front();
    }
}

