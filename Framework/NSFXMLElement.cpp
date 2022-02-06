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

#include "NSFXMLElement.h"

#include <fstream>

namespace NorthStateFramework
{
    // Public
    NSFXMLElement::NSFXMLElement()
        : parentElement(NULL)
    {
    }

    NSFXMLElement::NSFXMLElement(const NSFString& tag)
        : parentElement(NULL), tag(tag.c_str())
    {
    }

    NSFXMLElement::NSFXMLElement(const NSFString& tag, const NSFString& text)
        : parentElement(NULL), tag(tag.c_str()), text(text.c_str())
    {
        // Note: Trace data entries may not be valid XML for several reasons. One of them
        // involves the use of invalid characters, such as <, >, ", etc, which are handled
        // by escape().
        escape(this->text);
    }

    NSFXMLElement::NSFXMLElement(const NSFXMLElement& copyElement)
        : parentElement(NULL), tag(copyElement.tag.c_str()), text(copyElement.text.c_str())
    {
        NSFXMLElementIterator elementIterator;
        for (elementIterator = copyElement.childElements.begin(); elementIterator != copyElement.childElements.end(); ++ elementIterator)
        {
            addChildElementBack(new NSFXMLElement(*(*elementIterator)));
        }
    }

    NSFXMLElement::~NSFXMLElement()
    {
        while (!childElements.empty())
        {
            delete childElements.front();
            childElements.pop_front();
        }
    }

    void NSFXMLElement::addChildElementBack(NSFXMLElement* childElement)
    {
        childElement->parentElement = this;
        childElements.push_back(childElement);
    }

    void NSFXMLElement::addChildElementFront(NSFXMLElement* childElement)
    {
        childElement->parentElement = this;
        childElements.push_front(childElement);
    }

    bool NSFXMLElement::containsChildElement() const
    {
        return childElements.size() != 0;
    }

    bool NSFXMLElement::containsChildElement(const NSFString& childTag) const
    {
        NSFXMLElementIterator elementIterator;
        for (elementIterator = childElements.begin(); elementIterator != childElements.end(); ++ elementIterator)
        {
            if ((*elementIterator)->tag == childTag)
            {
                return true;
            }
        }

        return false;
    }

    void NSFXMLElement::deleteChildElement(NSFXMLElement* childElement)
    {
        childElements.remove(childElement);
        delete childElement;
    }

    bool NSFXMLElement::deleteChildElementBack()
    {
        if (childElements.size() == 0)
        {
            return false;
        }

        delete childElements.back();
        childElements.pop_back();

        return true;
    }

    bool NSFXMLElement::deleteChildElementFront()
    {
        if (childElements.size() == 0)
        {
            return false;
        }

        delete childElements.front();
        childElements.pop_front();

        return true;
    }

    NSFXMLElement* NSFXMLElement::getChildElement(const NSFString& childTag) const
    {
        NSFXMLElementIterator elementIterator;
        for (elementIterator = childElements.begin(); elementIterator != childElements.end(); ++elementIterator)
        {
            if ((*elementIterator)->tag == childTag)
            {
                return *elementIterator;
            }
        }

        return NULL;
    }

    NSFXMLElement* NSFXMLElement::getChildElementBack() const
    {
        if (childElements.size() == 0)
        {
            return NULL;
        }
        return childElements.back();
    }

    NSFXMLElement* NSFXMLElement::getChildElementFront() const
    {
        if (childElements.size() == 0)
        {
            return NULL;
        }
        return childElements.front();
    }

    NSFXMLElement* NSFXMLElement::getNextElement(NSFXMLElement* childElement) const
    {
        bool returnNext = false;

        NSFXMLElementIterator elementIterator;
        for (elementIterator = childElements.begin(); elementIterator != childElements.end(); ++elementIterator)
        {
            if (returnNext)
            {
                return (*elementIterator);
            }
            else if ((*elementIterator) == childElement)
            {
                returnNext = true;
            }
        }

        return NULL;
    }

    NSFXMLElement* NSFXMLElement::getNextElement(NSFXMLElement* childElement, const NSFString& nextTag) const
    {
        bool returnNext = false;

        NSFXMLElementIterator elementIterator;
        for (elementIterator = childElements.begin(); elementIterator != childElements.end(); ++elementIterator)
        {
            if (returnNext)
            {
                if ((*elementIterator)->getTag() == nextTag)
                {
                    return (*elementIterator);
                }
            }
            else if ((*elementIterator) == childElement)
            {
                returnNext = true;
            }
        }

        return NULL;
    }

    UInt32 NSFXMLElement::getNumberOfChildElements() const
    {
        return (UInt32) childElements.size();
    }

    NSFXMLElement* NSFXMLElement::getParentElement() const
    {
        return parentElement;
    }

    NSFXMLElement* NSFXMLElement::getParentElement(const NSFString& parentTag) const
    {
        if (parentElement == NULL)
        {
            return NULL;
        }

        if (parentElement->getTag() == parentTag)
        {
            return parentElement;
        }
        else
        {
            return parentElement->getParentElement(parentTag);
        }
    }

    const NSFString& NSFXMLElement::getTag() const
    {
        return tag;
    }

    const NSFString& NSFXMLElement::getText() const
    {
        return text;
    }

    bool NSFXMLElement::loadBuffer(const NSFString& buffer)
    {
        UInt32 readPosition = 0;
        return loadBuffer(buffer, readPosition);
    }

    void NSFXMLElement::save(std::ofstream& stream)
    {
        if (containsChildElement())
        {
            writeLine("<" + tag + ">", stream);

            NSFXMLElementIterator elementIterator;
            for (elementIterator = childElements.begin(); elementIterator != childElements.end(); ++elementIterator)
            {
                (*elementIterator)->save(stream);
            }

            writeLine("</" + tag + ">", stream);
        }
        else
        {
            writeLine ("<" + tag + ">" + text + "</" + tag + ">", stream);
        }
    }

    void NSFXMLElement::setTag(const NSFString& tag)
    {
        this->tag = tag.c_str();
    }

    void NSFXMLElement::setText(const NSFString& text)
    {
        this->text = text.c_str();
    }

    // Private Methods
    void NSFXMLElement::escape(NSFString& stringToEscape)
    {
        replaceCharWithString(stringToEscape, "&", "&amp;");
        replaceCharWithString(stringToEscape, "<", "&lt;");
        replaceCharWithString(stringToEscape, ">", "&gt;");
        replaceCharWithString(stringToEscape, "\"", "&quot;");
        replaceCharWithString(stringToEscape, "'", "&apos;");
    }

    bool NSFXMLElement::loadBuffer(const NSFString& buffer, UInt32& readPosition)
    {
        // Warning: buffer pointer is left modified

        if (!parseStartTag(buffer, readPosition))
        {
            return false;
        }

        // Check if start tag is also an end tag
        if (tag[tag.length() - 1] == '/')
        {
            tag = tag.erase(tag.length() - 1);
            return true;
        }

        if (!parseChildElements(buffer, readPosition))
        {
            return false;
        }

        if (childElements.size() == 0)
        {
            if (!parseText(buffer, readPosition))
            {
                return false;
            }
        }

        if (!parseEndTag(buffer, readPosition))
        {
            return false;
        }

        return true;
    }

    bool NSFXMLElement::parseAttributes(const NSFString& buffer, UInt32& readPosition)
    {
        // Read from buffer until end of tag
        while (readPosition < buffer.length())
        {
            // Check for end of tag
            if (buffer[readPosition] == '>')
            {
                ++readPosition;
                return true;
            }

            ++readPosition;
        }

        return false;
    }

    bool NSFXMLElement::parseChildElements(const NSFString& buffer, UInt32& readPosition)
    {
        while (true)
        {
            if (!skipWhitespace(buffer, readPosition))
            {
                return false;
            }

            if ((UInt32)buffer.size() < readPosition + 2)
            {
                return false;
            }

            // Check for child elements
            if ((buffer[readPosition] == '<') && (buffer[readPosition + 1] != '/'))
            {
                NSFXMLElement* newElement = new NSFXMLElement();

                if (!newElement->loadBuffer(buffer, readPosition))
                {
                    delete newElement;
                    return false;
                }

                addChildElementBack(newElement);

                continue;
            }

            return true;
        }
    }

    bool NSFXMLElement::parseEndTag(const NSFString& buffer, UInt32& readPosition)
    {
        if (!skipWhitespace(buffer, readPosition))
        {
            return false;
        }

        if ((UInt32)buffer.size() < readPosition + 2)
        {
            return false;
        }

        // Check for start of end tag
        if ((buffer[readPosition] != '<') || (buffer[readPosition + 1] != '/'))
        {
            return false;
        }

        // Increment past start of end tag
        readPosition += 2;

        // Read end tag
        NSFString endTag;
        while (true)
        {
            // Check for end of buffer
            if (readPosition >= buffer.length())
            {
                return false;
            }

            // Check for end of end tag
            if (buffer[readPosition] == '>')
            {
                ++readPosition;
                break;
            }

            endTag += buffer[readPosition];

            ++readPosition;
        }

        // Verify end tag
        if (endTag != tag)
        {
            return false;
        }

        return true;
    }

    bool NSFXMLElement::parseEntity(const NSFString& buffer, UInt32& readPosition)
    {
        NSFString entityBuffer;

        //Read in entity
        const int MaxEntityLength = 5;
        for (int i = 0; i < MaxEntityLength; ++i)
        {
            // readPosition should be pointing to '&' at beginning of entity
            ++readPosition;

            entityBuffer += buffer[readPosition];

            // Check for end of entity
            if (buffer[readPosition] == ';')
            {
                if (entityBuffer.substr(0, 2) == "lt")
                {
                    text += '<';
                }
                else if (entityBuffer.substr(0, 2) == "#x")
                {
                    text += (char) strtol(&entityBuffer[2], NULL, 16);
                }
                else if (entityBuffer.substr(0, 1) == "#")
                {
                    text += (char) strtol(&entityBuffer[1], NULL, 10);
                }
                else  // Unrecognized entity
                {
                    return false;
                }

                ++readPosition;

                return true;
            }
        }

        return false;
    }

    bool NSFXMLElement::parseStartTag(const NSFString& buffer, UInt32& readPosition)
    {
        // Read from buffer until beginning of tag
        while (true)
        {
            // Check for end of buffer
            if (readPosition >= buffer.length())
            {
                return false;
            }

            // Check for beginning of tag
            if (buffer[readPosition] == '<')
            {
                ++readPosition;

                // Check for declaration or comment
                if ((buffer[readPosition] == '?') || (buffer[readPosition] == '!'))
                {
                    ++readPosition;
                    continue;
                }
                else  // It's a tag
                {
                    break;
                }
            }

            ++readPosition;
        }

        if (!skipWhitespace(buffer, readPosition))
        {
            return false;
        }

        while (true)
        {
            // Check for end of buffer
            if (readPosition >= buffer.length())
            {
                return false;
            }

            // Check for end of tag
            if (buffer[readPosition] == '>')
            {
                ++readPosition;
                return true;
            }

            if (isspace((UInt8)buffer[readPosition]))
            {
                // WARNING - Current implementation does not save attributes!
                if (!parseAttributes(buffer, readPosition))
                {
                    return false;
                }

                return true;
            }

            tag += buffer[readPosition];

            ++readPosition;
        }
    }

    bool NSFXMLElement::parseText(const NSFString& buffer, UInt32& readPosition)
    {
        if (!skipWhitespace(buffer, readPosition))
        {
            return false;
        }

        // Read from buffer until new tag
        while (true)
        {
            // Check for end of buffer
            if (readPosition >= buffer.length())
            {
                return false;
            }

            // Check for beginning of end tag
            if (buffer[readPosition] == '<')
            {
                return true;
            }

            // Check for entity
            // The parser only supports &lt, &#, and &#x
            if (buffer[readPosition] == '&')
            {
                if (!parseEntity(buffer, readPosition))
                {
                    return false;
                }

                continue;
            }

            text += buffer[readPosition];

            ++readPosition;
        }
    }

    void NSFXMLElement::replaceCharWithString(NSFString& stringToModify, const NSFString& characterToReplace, const NSFString& stringToInsert)
    {
        size_t positionFound = stringToModify.find(characterToReplace);

        while (positionFound != NSFString::npos)
        {
            stringToModify.replace(positionFound, characterToReplace.size(), stringToInsert);
            if (positionFound == stringToModify.size() -1) return;
            positionFound = stringToModify.find(characterToReplace, positionFound + 1); // start up at the next spot after the character just replaced
        }
    }

    bool NSFXMLElement::skipWhitespace(const NSFString& buffer, UInt32& readPosition)
    {
        while (true)
        {
            // Check for end of buffer
            if (readPosition >= buffer.length())
            {
                return false;
            }

            if (!isspace((UInt8)buffer[readPosition]))
            {
                return true;
            }

            ++readPosition;
        }
    }

    void NSFXMLElement::writeLine(const NSFString& string, std::ofstream& stream)
    {
        stream << string << std::endl;
    }
}
