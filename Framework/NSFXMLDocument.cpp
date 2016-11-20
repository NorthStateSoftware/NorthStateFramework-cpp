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

#include "NSFXMLDocument.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>

namespace NorthStateFramework
{
    // Public
    NSFXMLDocument::NSFXMLDocument()
        : errorStatus(NoError), bookmarkElement(NULL), currentElement(NULL), rootElement(NULL)
    {
    }

    NSFXMLDocument::NSFXMLDocument(const NSFString& rootTag)
        : errorStatus(NoError), bookmarkElement(NULL), currentElement(NULL), rootElement(NULL)
    {
        addRootElement(rootTag);
    }

    NSFXMLDocument::NSFXMLDocument(const NSFXMLDocument& copyDocument)
        : errorStatus(NoError), bookmarkElement(NULL), currentElement(NULL), rootElement(NULL)
    {
        loadDocument(copyDocument);
    }

    NSFXMLDocument::~NSFXMLDocument()
    {
        delete rootElement;
    }

    void NSFXMLDocument::addChildElementBack(const NSFString& childTag)
    {
        if (rootElement == NULL)
        {
            rootElement = new NSFXMLElement(childTag);
            currentElement = rootElement;
            bookmarkElement = rootElement;
        }
        else
        {
            currentElement->addChildElementBack(new NSFXMLElement(childTag));
        }
    }

    void NSFXMLDocument::addChildElementBack(const NSFString& childTag, const NSFString& childText)
    {
        if (rootElement == NULL)
        {
            rootElement = new NSFXMLElement(childTag, childText);
            currentElement = rootElement;
            bookmarkElement = rootElement;
        }
        else
        {
            currentElement->addChildElementBack(new NSFXMLElement(childTag, childText));
        }
    }

    bool NSFXMLDocument::addChildElementBack(const NSFXMLDocument& document)
    {
        if (document.rootElement == NULL)
        {
            errorStatus = EmptyElement;
            return false;
        }

        if (rootElement == NULL)
        {
            rootElement = new NSFXMLElement(*document.rootElement);
            currentElement = rootElement;
            bookmarkElement = rootElement;
        }
        else
        {
            currentElement->addChildElementBack(new NSFXMLElement(*document.rootElement));
        }

        return true;
    }

    bool NSFXMLDocument::addChildElementBack(NSFXMLElement* childElement)
    {
        if (rootElement == NULL)
        {
            rootElement = childElement;
            currentElement = rootElement;
            bookmarkElement = rootElement;
            return true;
        }

        currentElement->addChildElementBack(childElement);
        return true;
    }

    void NSFXMLDocument::addChildElementFront(const NSFString& childTag)
    {
        if (rootElement == NULL)
        {
            rootElement = new NSFXMLElement(childTag);
            currentElement = rootElement;
            bookmarkElement = rootElement;
        }
        else
        {
            currentElement->addChildElementFront(new NSFXMLElement(childTag));
        }
    }

    void NSFXMLDocument::addChildElementFront(const NSFString& childTag, const NSFString& childText)
    {
        if (rootElement == NULL)
        {
            rootElement = new NSFXMLElement(childTag, childText);
            currentElement = rootElement;
            bookmarkElement = rootElement;
        }
        else
        {
            currentElement->addChildElementFront(new NSFXMLElement(childTag, childText));
        }
    }

    bool NSFXMLDocument::addChildElementFront(const NSFXMLDocument& document)
    {
        if (document.rootElement == NULL)
        {
            errorStatus = EmptyElement;
            return false;
        }

        if (rootElement == NULL)
        {
            rootElement = new NSFXMLElement(*document.rootElement);
            currentElement = rootElement;
            bookmarkElement = rootElement;
        }
        else
        {
            currentElement->addChildElementFront(new NSFXMLElement(*document.rootElement));
        }

        return true;
    }

    bool NSFXMLDocument::addChildElementFront(NSFXMLElement* childElement)
    {
        if (rootElement == NULL)
        {
            rootElement = childElement;
            currentElement = rootElement;
            bookmarkElement = rootElement;
            return true;
        }

        currentElement->addChildElementFront(childElement);
        return true;
    }

    void NSFXMLDocument::addRootElement(const NSFString& rootTag)
    {
        NSFXMLElement* newElement = new NSFXMLElement(rootTag, emptyString());

        if (rootElement != NULL)
        {
            newElement->addChildElementFront(rootElement);
        }

        rootElement = newElement;
        bookmarkElement = rootElement;
        currentElement = rootElement;
    }

    bool NSFXMLDocument::atBookmarkElement() const
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        return (currentElement == bookmarkElement);
    }

    bool NSFXMLDocument::atRootElement() const
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        return (currentElement == rootElement);
    }

    bool NSFXMLDocument::containsChildElement() const
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        return currentElement->containsChildElement();
    }

    bool NSFXMLDocument::containsChildElement(const NSFString& childTag) const
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        return currentElement->containsChildElement(childTag);
    }

    bool NSFXMLDocument::deleteChildElementBack()
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        if (bookmarkElement == currentElement->getChildElementBack())
        {
            bookmarkElement = NULL;
        }

        return currentElement->deleteChildElementBack();
    }

    bool NSFXMLDocument::deleteChildElementFront()
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        if (bookmarkElement == currentElement->getChildElementFront())
        {
            bookmarkElement = NULL;
        }

        return currentElement->deleteChildElementFront();
    }

    bool NSFXMLDocument::deleteCurrentElement()
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        if (currentElement == rootElement)
        {
            rootElement = NULL;
            bookmarkElement = NULL;
            currentElement = NULL;

            return true;
        }

        if (bookmarkElement == currentElement)
        {
            bookmarkElement = NULL;
        }

        NSFXMLElement* deleteElement = currentElement;
        currentElement = currentElement->getParentElement();
        currentElement->deleteChildElement(deleteElement);

        return true;
    }

    bool NSFXMLDocument::getChildElementBoolean(const NSFString& childTag, bool& value)
    {
        errorStatus = NoError;

        if (jumpToChildElement(childTag))
        {
            getCurrentElementBoolean(value);
            jumpToParentElement();
            return true;
        }
        else
        {
            errorStatus = EmptyElement;
            return false;
        }
    }

    void NSFXMLDocument::getChildElementBoolean(const NSFString& childTag, bool& value, bool defaultValue)
    {
        if (!getChildElementBoolean(childTag, value))
        {
            value = defaultValue;
        }
    }

    bool NSFXMLDocument::getChildElementText(const NSFString& childTag, NSFString& text)
    {
        errorStatus = NoError;

        if (jumpToChildElement(childTag))
        {
            getCurrentElementText(text);
            jumpToParentElement();
            return true;
        }
        else
        {
            errorStatus = EmptyElement;
            return false;
        }
    }

    void NSFXMLDocument::getChildElementText(const NSFString& childTag, NSFString& text, const NSFString& defaultText)
    {
        if (!getChildElementText(childTag, text))
        {
            text = defaultText;
        }
    }

    bool NSFXMLDocument::getCurrentElementBoolean(bool& value)
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        NSFString booleanString = currentElement->getText().c_str();

        std::transform(booleanString.begin(), booleanString.end(), booleanString.begin(), tolower);

        if ((booleanString == "true") || (booleanString == "t") || (booleanString == "1"))
        {
            value = true;

            return true;
        }
        else if ((booleanString == "false") || (booleanString == "f") || (booleanString == "0"))
        {
            value = false;

            return true;
        }

        errorStatus = BadValue;
        return false;
    }

    bool NSFXMLDocument::getCurrentElementTag(NSFString& tag) const
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        tag = currentElement->getTag();

        return true;
    }

    bool NSFXMLDocument::getCurrentElementText(NSFString& text) const
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        text = currentElement->getText();

        return true;
    }

    bool NSFXMLDocument::getNumberOfChildElements(UInt32& numberOfChildElements) const
    {
        numberOfChildElements = 0;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        numberOfChildElements = currentElement->getNumberOfChildElements();
        return true;
    }

    bool NSFXMLDocument::jumpToBookmarkElement()
    {
        errorStatus = NoError;

        if (bookmarkElement == NULL)
        {
            errorStatus = EmptyElement;
            return false;
        }

        currentElement = bookmarkElement;

        return true;
    }


    bool NSFXMLDocument::jumpToChildElement(const NSFString& childTag)
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        NSFXMLElement* childElement = currentElement->getChildElement(childTag);

        if (childElement == NULL)
        {
            errorStatus = EmptyElement;
            return false;
        }

        currentElement = childElement;

        return true;
    }

    bool NSFXMLDocument::jumpToChildElementBack()
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        NSFXMLElement* childElement = currentElement->getChildElementBack();

        if (childElement == NULL)
        {
            errorStatus = EmptyElement;
            return false;
        }

        currentElement = childElement;

        return true;
    }

    bool NSFXMLDocument::jumpToChildElementFront()
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        NSFXMLElement* childElement = currentElement->getChildElementFront();

        if (childElement == NULL)
        {
            errorStatus = EmptyElement;
            return false;
        }

        currentElement = childElement;

        return true;
    }

    bool NSFXMLDocument::jumpToNextElement()
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        if (currentElement == rootElement)
        {
            errorStatus = EmptyElement;
            return false;
        }

        NSFXMLElement* nextElement = currentElement->getParentElement()->getNextElement(currentElement);

        if (nextElement == NULL)
        {
            errorStatus = EmptyElement;
            return false;
        }

        currentElement = nextElement;

        return true;
    }

    bool NSFXMLDocument::jumpToNextElement(const NSFString& nextTag)
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        if (currentElement == rootElement)
        {
            errorStatus = EmptyElement;
            return false;
        }

        NSFXMLElement* nextElement = currentElement->getParentElement()->getNextElement(currentElement, nextTag);

        if (nextElement == NULL)
        {
            errorStatus = EmptyElement;
            return false;
        }

        currentElement = nextElement;

        return true;
    }

    bool NSFXMLDocument::jumpToParentElement()
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        if (currentElement == rootElement)
        {
            errorStatus = EmptyElement;
            return false;
        }

        currentElement = currentElement->getParentElement();

        return true;
    }

    bool NSFXMLDocument::jumpToParentElement(const NSFString& parentTag)
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        NSFXMLElement* parentElement = currentElement->getParentElement(parentTag);

        if (parentElement == NULL)
        {
            errorStatus = EmptyElement;
            return false;
        }

        currentElement = parentElement;

        return true;
    }

    bool NSFXMLDocument::jumpToRootElement()
    {
        errorStatus = NoError;

        currentElement = rootElement;
        return true;
    }

    bool NSFXMLDocument::loadBuffer(const NSFString& buffer)
    {
        errorStatus = NoError;

        delete rootElement;

        rootElement = NULL;
        bookmarkElement = NULL;
        currentElement = NULL;

        rootElement = new NSFXMLElement();

        if (!rootElement->loadBuffer(buffer))
        {
            delete rootElement;
            rootElement = NULL;
            errorStatus = ParseError;
            return false;
        }

        bookmarkElement = rootElement;
        currentElement = rootElement;

        return true;
    }

    void NSFXMLDocument::loadDocument(const NSFXMLDocument& copyDocument)
    {
        delete rootElement;

        rootElement = NULL;
        bookmarkElement = NULL;
        currentElement = NULL;

        if (copyDocument.rootElement != NULL)
        {
            rootElement = new NSFXMLElement(*copyDocument.rootElement);
            bookmarkElement = rootElement;
            currentElement = rootElement;
        }
    }

    bool NSFXMLDocument::loadFile(const NSFString& fileName)
    {
        errorStatus = NoError;

        delete rootElement;

        rootElement = NULL;
        bookmarkElement = NULL;
        currentElement = NULL;

        // C++ streams by default do not throw exceptions, so no try/catch necessary

        std::ifstream  file;
        file.open(fileName.c_str());
        NSFString* buffer = readToEnd(file);
        file.close();

        if (buffer == NULL)
        {
            // Nothing to delete
            return false;
        }

        bool returnValue = loadBuffer(*buffer);

        delete buffer;

        return returnValue;
    }

    void NSFXMLDocument::reset()
    {
        bookmarkElement = rootElement;
        currentElement = rootElement;
    }

    bool NSFXMLDocument::save(const NSFString& fileName)
    {
        errorStatus = NoError;

        if (rootElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        // C++ streams by default do not throw exceptions, so no try/catch necessary

        std::ofstream file;
        file.open(fileName.c_str(), std::ios_base::trunc);

        if (file.fail())
        {
            throw std::runtime_error("Unable to open file: " + fileName);
        }

        rootElement->save(file);
        file.close();
        return true;
    }

    void NSFXMLDocument::setBookmarkElement()
    {
        bookmarkElement = currentElement;
    }

    void NSFXMLDocument::setChildElementText(const NSFString& childTag, const NSFString& text)
    {
        if (jumpToChildElement(childTag))
        {
            currentElement->setText(text);
            jumpToParentElement();
        }
        else
        {
            addChildElementBack(childTag, text);
        }
    }

    bool NSFXMLDocument::setCurrentElementTag(const NSFString& tag)
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        currentElement->setTag(tag);

        return true;
    }

    bool NSFXMLDocument::setCurrentElementText(const NSFString& text)
    {
        errorStatus = NoError;

        if (currentElement == NULL)
        {
            errorStatus = EmptyDocument;
            return false;
        }

        currentElement->setText(text);

        return true;
    }

    // Private

    NSFString* NSFXMLDocument::readToEnd(std::ifstream& file)
    {
        // get length of file:
        file.seekg(0, std::ios::end);
        int length = (int) file.tellg();
        file.seekg(0, std::ios::beg);

        // Check that the file had a valid length
        if (length == -1)
        {
            return NULL;
        }

        // allocate memory:
        char* buffer = new char[length];

        // read data as a block:
        file.read(buffer,length);

        NSFString* string = new NSFString(buffer, length);

        delete[] buffer;
        return string;
    }
}
