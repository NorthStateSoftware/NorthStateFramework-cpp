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

#ifndef NSF_XML_DOCUMENT_H
#define NSF_XML_DOCUMENT_H

#include "NSFTaggedTypes.h"
#include "NSFXMLElement.h"

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a simple in-memory xml document editor.
    /// </summary>
    /// <remarks>
    /// This class is not thread safe.
    /// </remarks>
    class NSFXMLDocument
    {
    public:

        /// <summary>
        /// Represents xml document error status.
        /// </summary>
        enum ErrorStatus { NoError, EmptyDocument, EmptyElement, ParseError, FileError, BadValue };

        /// <summary>
        /// Creates an xml document.
        /// </summary>
        NSFXMLDocument();

        /// <summary>
        /// Creates an xml document.
        /// </summary>
        /// <param name="rootTag">The string for the root element of the document.</param>
        NSFXMLDocument(const NSFString& rootTag);

        /// <summary>
        /// Creates an xml document.
        /// </summary>
        /// <param name="copyDocument">The document to copy.</param>
        NSFXMLDocument(const NSFXMLDocument& copyDocument);

        /// <summary>
        /// Destroys and xml document
        /// </summary>
        ~NSFXMLDocument();

        /// <summary>
        /// Adds an element to the back of the current element's child elements
        /// </summary>
        /// <param name="childTag">The tag for the element.</param>
        void addChildElementBack(const NSFString& childTag);

        /// <summary>
        /// Adds an element to the back of the current element's child elements
        /// </summary>
        /// <param name="childTag">The tag for the element.</param>
        /// <param name="childText">The text for the element.</param>
        void addChildElementBack(const NSFString& childTag, const NSFString& childText);

        /// <summary>
        /// Adds an element to the back of the current element's child elements
        /// </summary>
        /// <param name="childTag">The tag for the element.</param>
        /// <param name="value">The value (converted to string) for the element.</param>
        template<class ValueType> void addChildElementBack(const NSFString& childTag, ValueType value)
        {
            NSFString childText = toString<ValueType>(value);
            addChildElementBack(childTag, childText);
        }

        /// <summary>
        /// Adds an xml document to the back of the current element's child elements
        /// </summary>
        /// <param name="document">The document to add.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool addChildElementBack(const NSFXMLDocument& document);

        /// <summary>
        /// Adds an element to the back of the current element's child elements
        /// </summary>
        /// <param name="childElement">The element to add.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool addChildElementBack(NSFXMLElement* childElement);

        /// <summary>
        /// Adds an element to the front of the current element's child elements
        /// </summary>
        /// <param name="childTag">The tag for the element.</param>
        void addChildElementFront(const NSFString& childTag);

        /// <summary>
        /// Adds an element to the front of the current element's child elements
        /// </summary>
        /// <param name="childTag">The tag for the element.</param>
        /// <param name="childText">The text for the element.</param>
        void addChildElementFront(const NSFString& childTag, const NSFString& childText);

        /// <summary>
        /// Adds an element to the front of the current element's child elements
        /// </summary>
        /// <param name="childTag">The tag for the element.</param>
        /// <param name="value">The value (converted to string) for the element.</param>
        template<class ValueType> void addChildElementFront(const NSFString& childTag, ValueType value)
        {
            NSFString childText = toString<ValueType>(value);
            addChildElementFront(childTag, childText);
        }

        /// <summary>
        /// Adds an xml document to the front of the current element's child elements
        /// </summary>
        /// <param name="document">The document to add.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool addChildElementFront(const NSFXMLDocument& document);

        /// <summary>
        /// Adds an element to the front of the current element's child elements
        /// </summary>
        /// <param name="childElement">The element to add.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool addChildElementFront(NSFXMLElement* childElement);

        /// <summary>
        /// Adds a root element and sets the current element and bookmark element to the root element.
        /// </summary>
        void addRootElement(const NSFString& rootTag);

        /// <summary>
        /// Checks if at the bookmark element.
        /// </summary>
        /// <returns>True if at bookmark element, false otherwise.</returns>
        bool atBookmarkElement() const;

        /// <summary>
        /// Checks if at the root element.
        /// </summary>
        /// <returns>True if at root element, false otherwise.</returns>
        bool atRootElement() const ;

        /// <summary>
        /// Checks if the current elements contains a child element.
        /// </summary>
        /// <returns>True if current element contains a child element, false otherwise.</returns>
        bool containsChildElement() const;

        /// <summary>
        /// Checks if the current elements contains a child element with the specified tag.
        /// </summary>
        /// <param name="childTag">The tag for the child element.</param>
        /// <returns>True if current element contains a child element with the specified tag, false otherwise.</returns>
        bool containsChildElement(const NSFString& childTag) const;

        /// <summary>
        /// Deletes the current element's last child element.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool deleteChildElementBack();

        /// <summary>
        /// Deletes the current element's first child element.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool deleteChildElementFront();

        /// <summary>
        /// Deletes the current element.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool deleteCurrentElement();

        /// <summary>
        /// Gets the boolean value of the child element with the specified tag.
        /// </summary>
        /// <param name="childTag">The child element's tag.</param>
        /// <param name="value">The child element's boolean value.</param>
        /// <returns>True if successful, false otherwise.</returns>
        /// <remarks>
        /// This operation looks for (case insensitive): "true", "t", or "1" for true; "false", "f", or "0" for false.
        /// </remarks>
        bool getChildElementBoolean(const NSFString& childTag, bool& value);

        /// <summary>
        /// Gets the boolean value of the child element with the specified tag or the default value if no child found.
        /// </summary>
        /// <param name="childTag">The child element's tag.</param>
        /// <param name="value">The child element's boolean value.</param>
        /// <param name="defaultValue">The default value returned in value argument if child not found.</param>
        /// <remarks>
        /// This operation looks for (case insensitive): "true", "t", or "1" for true; "false", "f", or "0" for false.
        /// </remarks>
        void getChildElementBoolean(const NSFString& childTag, bool& value, const bool defaultValue);

        /// <summary>
        /// Gets the text of the child element with the specified tag.
        /// </summary>
        /// <param name="childTag">The child element's tag.</param>
        /// <param name="text">The child element's text.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool getChildElementText(const NSFString& childTag, NSFString& text);

        /// <summary>
        /// Gets the text of the child element with the specified tag or the default text if no child found.
        /// </summary>
        /// <param name="childTag">The child element's tag.</param>
        /// <param name="text">The child element's text.</param>
        /// <param name="defaultText">The default text returned in text argument if child not found.</param>
        void getChildElementText(const NSFString& childTag, NSFString& text, const NSFString& defaultText);

        /// <summary>
        /// Gets the value of the child element with the specified tag.
        /// </summary>
        /// <param name="childTag">The child element's tag.</param>
        /// <param name="value">The child element's value.</param>
        /// <returns>True if successful, false otherwise.</returns>
        template<class ValueType> bool getChildElementValue(const NSFString& childTag, ValueType& value)
        {
            errorStatus = NoError;

            if (jumpToChildElement(childTag))
            {
                getCurrentElementValue(value);
                jumpToParentElement();
                return true;
            }
            else
            {
                errorStatus = EmptyElement;
                return false;
            }
        }

        /// <summary>
        /// Gets the value of the child element with the specified tag or the default value if no child found.
        /// </summary>
        /// <param name="childTag">The child element's tag.</param>
        /// <param name="value">The child element's value.</param>
        /// <param name="defaultValue">The default value returned in value argument if child not found.</param>
        template<class ValueType> void getChildElementValue(const NSFString& childTag, ValueType& value, ValueType defaultValue)
        {
            if (!getChildElementValue(childTag, value))
            {
                value = defaultValue;
            }
        }

        /// <summary>
        /// Gets the current element's boolean value.
        /// </summary>
        /// <param name="value">The current element's boolean value.</param>
        /// <returns>True if successful, false otherwise.</returns>
        /// <remarks>
        /// This operation looks for (case insensitive): "true", "t", or "1" for true; "false", "f", or "0" for false.
        /// </remarks>
        bool getCurrentElementBoolean(bool& value);

        /// <summary>
        /// Gets the current element's tag.
        /// </summary>
        /// <param name="tag">The current element's tag.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool getCurrentElementTag(NSFString& tag) const;

        /// <summary>
        /// Gets the current element's text.
        /// </summary>
        /// <param name="text">The current element's text.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool getCurrentElementText(NSFString& text) const;

        /// <summary>
        /// Gets the current element's integer value.
        /// </summary>
        /// <param name="value">The current element's value.</param>
        /// <returns>True if successful, false otherwise.</returns>
        template<class ValueType> bool getCurrentElementValue(ValueType& value) const
        {
            errorStatus = NoError;

            if (currentElement == NULL)
            {
                errorStatus = EmptyDocument;
                return false;
            }

            value = atoi(currentElement->getText().data());

            return true;
        }

        /// <summary>
        /// Gets the error status of the last operation.
        /// </summary>
        /// <remarks>
        /// Boolean methods that return false set the error status of the document.
        /// </remarks>
        ErrorStatus getErrorStatus() const { return errorStatus; };

        /// <summary>
        /// Gets the number of child elements.
        /// </summary>
        /// <param name="numberOfChildElements">The number of child elements.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool getNumberOfChildElements(UInt32& numberOfChildElements) const;

        /// <summary>
        /// Moves the current element pointer to the bookmarked element.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        /// <remarks>
        /// The bookmark provides a mechanism for jumping back to a specific element in the document.
        /// </remarks>
        bool jumpToBookmarkElement();

        /// <summary>
        /// Moves the current element pointer to the first child element with the specified tag.
        /// </summary>
        /// <param name="childTag">The child tag to find.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool jumpToChildElement(const NSFString& childTag);

        /// <summary>
        /// Moves the current element pointer to the last child element of the current element.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool jumpToChildElementBack();

        /// <summary>
        /// Moves the current element pointer to the first child element of the current element.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool jumpToChildElementFront();

        /// <summary>
        /// Moves the current element pointer to the next element at the same level.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool jumpToNextElement();

        /// <summary>
        /// Moves the current element pointer to the next element with the specified tag at the same level.
        /// </summary>
        /// <param name="nextTag">The tag to find.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool jumpToNextElement(const NSFString& nextTag);

        /// <summary>
        /// Moves the current element pointer to the parent of the current element.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool jumpToParentElement();

        /// <summary>
        /// Moves the current element pointer to the parent of the current element with the specified tag.
        /// </summary>
        /// <param name="parentTag">The tag to find.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool jumpToParentElement(const NSFString& parentTag);

        /// <summary>
        /// Moves the current element pointer to the root element.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool jumpToRootElement();

        /// <summary>
        /// Populates the document with xml formatted text in the specified buffer.
        /// </summary>
        /// <param name="buffer">The buffer to load.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool loadBuffer(const NSFString& buffer);

        /// <summary>
        /// Populates the document with xml from specified xml document.
        /// </summary>
        /// <param name="copyDocument">The file to copy.</param>
        void loadDocument(const NSFXMLDocument& copyDocument);

        /// <summary>
        /// Populates the document with xml formatted text in the specified file.
        /// </summary>
        /// <param name="fileName">The file to load.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool loadFile(const NSFString& fileName);

        /// <summary>
        /// Sets the current and bookmark element to the root element.
        /// </summary>
        void reset();

        /// <summary>
        /// Saves the xml document to the specified file name.
        /// </summary>
        /// <param name="fileName">The name of the file.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool save(const NSFString& fileName);

        /// <summary>
        /// Sets the bookmark element to the current element.
        /// </summary>
        /// <remarks>
        /// The bookmark provides a mechanism for jumping back to a specific element in the document.
        /// </remarks>
        void setBookmarkElement();

        /// <summary>
        /// Sets the text of the specified child element or creates a child element if no match is found.
        /// </summary>
        /// <param name="childTag">The child element's tag.</param>
        /// <param name="text">The child element's text.</param>
        void setChildElementText(const NSFString& childTag, const NSFString& text);

        /// <summary>
        /// Sets the text of the specified child element to the specified value, converted to a string, or creates a child element if no match is found.
        /// </summary>
        /// <param name="childTag">The child element's tag.</param>
        /// <param name="value">The child element's value.</param>
        template<class ValueType> void setChildElementValue(const NSFString& childTag, ValueType value)
        {
            if (jumpToChildElement(childTag))
            {
                setCurrentElementValue(value);
                jumpToParentElement();
            }
            else
            {
                addChildElementBack(childTag, value);
            }
        }

        /// <summary>
        /// Sets the current element's tag.
        /// </summary>
        /// <param name="tag">The current element's tag.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool setCurrentElementTag(const NSFString& tag);

        /// <summary>
        /// Sets the current element's text.
        /// </summary>
        /// <param name="text">The current element's text.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool setCurrentElementText(const NSFString& text);

        /// <summary>
        /// Sets the current element's text to the specified value, converted to a string.
        /// </summary>
        /// <param name="value">The current element's value.</param>
        /// <returns>True if successful, false otherwise.</returns>
        template<class ValueType> bool setCurrentElementValue(ValueType value)
        {
            errorStatus = NoError;

            if (currentElement == NULL)
            {
                errorStatus = EmptyDocument;
                return false;
            }

            currentElement->setText(toString<ValueType>(value));

            return true;
        }

    private:

        mutable ErrorStatus errorStatus;

        NSFXMLElement* bookmarkElement;
        NSFXMLElement* currentElement;
        NSFXMLElement* rootElement;

        NSFString* readToEnd(std::ifstream& file);
    };
}

#endif // NSF_XML_DOCUMENT_H
