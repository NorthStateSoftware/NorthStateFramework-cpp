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

#include "DocumentNavigationTest.h"

using namespace NorthStateFramework;

namespace NSFTest
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    DocumentNavigationTest::DocumentNavigationTest(const NSFString& name, string fileName)
        :name(name.c_str()), fileName(fileName.c_str())
    {
    }

    bool DocumentNavigationTest::runTest(NSFString& errorMessage)
    {
        int testValue;
        NSFXMLDocument* document = new NSFXMLDocument();

        if (!document->loadFile(fileName))
        {
            errorMessage = "Error loading file";
            delete document;
            return false;
        }

        document->getChildElementValue<int>("VE_1_1", testValue);
        if (testValue != 5)
        {
            errorMessage = "Error reading value VE_1_1";
            delete document;
            return false;
        }

        if (!document->jumpToChildElement("CE_1_1"))
        {
            errorMessage = "Error moving to child element CE_1_1";
            delete document;
            return false;
        }

        if (!document->jumpToChildElement("CE_1_1_1"))
        {
            errorMessage = "Error moving to child element CE_1_1_1";
            delete document;
            return false;
        }

        document->getChildElementValue<int>("VE_1_1_1_1", testValue);
        if (testValue != 1)
        {
            errorMessage = "Error reading value VE_1_1_1_1";
            delete document;
            return false;
        }

        if (!document->jumpToParentElement())
        {
            errorMessage = "Error moving back up to parent element CE_1_1_1";
            delete document;
            return false;
        }

        if (!document->jumpToParentElement())
        {
            errorMessage = "Error moving back up to parent element CE_1_1";
            delete document;
            return false;
        }

        document->getChildElementValue<int>("VE_1_1", testValue);
        if (testValue != 5)
        {
            errorMessage = "Error reading value VE_1_1 second time";
            delete document;
            return false;
        }

        delete document;
        return true;
    }
}