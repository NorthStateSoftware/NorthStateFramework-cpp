// Copyright 2004-2014, North State Software, LLC.  All rights reserved.

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