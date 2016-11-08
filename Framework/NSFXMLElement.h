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

#ifndef NSF_XML_ELEMENT_H
#define NSF_XML_ELEMENT_H

#include "NSFTaggedTypes.h"

namespace NorthStateFramework
{
    class NSFXMLElement;
    typedef std::list<NSFXMLElement*> NSFXMLElementList;
    typedef NSFXMLElementList::const_iterator NSFXMLElementIterator;

    /// <summary>
    /// Represents an xml element in an xml document.
    /// </summary>
    /// <remarks>
    /// This is a companion class to NSFXMLDocument.
    /// </remarks>
    class NSFXMLElement
    {
    public:

        /// <summary>
        /// Creates an xml element.
        /// </summary>
        NSFXMLElement();

        /// <summary>
        /// Creates an xml element.
        /// </summary>
        /// <param name="tag">The tag for the element.</param>
        NSFXMLElement(const NSFString& tag);

        /// <summary>
        /// Creates an xml element.
        /// </summary>
        /// <param name="tag">The tag for the element.</param>
        /// <param name="text">The text for the element.</param>
        NSFXMLElement(const NSFString& tag, const NSFString& text);

        /// <summary>
        /// Creates an xml element.
        /// </summary>
        /// <param name="copyElement">The element to copy.</param>
        NSFXMLElement(const NSFXMLElement& copyElement);

        /// <summary>
        /// Destroys an xml element.
        /// </summary>
        ~NSFXMLElement();

        /// <summary>
        /// Adds an element to the back of the child element list.
        /// </summary>
        void addChildElementBack(NSFXMLElement* childElement);

        /// <summary>
        /// Adds an element to the front of the child element list.
        /// </summary>
        void addChildElementFront(NSFXMLElement* childElement);

        /// <summary>
        /// Checks if the element contains any child elements.
        /// </summary>
        /// <returns>True if any child elements, false otherwise.</returns>
        bool containsChildElement() const;

        /// <summary>
        /// Checks if the element contains any child elements with the specified tag.
        /// </summary>
        /// <param name="childTag">The child element tag.</param>
        /// <returns>True if contains, false otherwise.</returns>
        bool containsChildElement(const NSFString& childTag) const;

        /// <summary>
        /// Deletes the specified child element.
        /// </summary>
        /// <param name="childElement">The element to delete.</param>
        void deleteChildElement(NSFXMLElement* childElement);

        /// <summary>
        /// Deletes the element at the back of the child element list.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool deleteChildElementBack();

        /// <summary>
        /// Deletes the element at the front of the child element list.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool deleteChildElementFront();

        /// <summary>
        /// Gets the first child element with the specified tag.
        /// </summary>
        /// <param name="childTag">The child element to tag.</param>
        /// <returns>The child element, or NULL if none exist.</returns>
        NSFXMLElement* getChildElement(const NSFString& childTag) const;

        /// <summary>
        /// Gets the last child element.
        /// </summary>
        /// <returns>The child element, or NULL if none exist.</returns>
        NSFXMLElement* getChildElementBack() const;

        /// <summary>
        /// Gets the first child element.
        /// </summary>
        /// <returns>The child element, or NULL if none exist.</returns>
        NSFXMLElement* getChildElementFront() const;

        /// <summary>
        /// Gets the next child element after the specified child element.
        /// </summary>
        /// <param name="childElement">The child element before the returned child element.</param>
        /// <returns>The child element, or NULL if none exist.</returns>
        NSFXMLElement* getNextElement(NSFXMLElement* childElement) const;

        /// <summary>
        /// Gets the next child element after the specified child element, with the specified tag.
        /// </summary>
        /// <param name="childElement">The child element before the returned child element.</param>
        /// <param name="nextTag">The child element tag.</param>
        /// <returns>The child element, or NULL if none exist.</returns>
        NSFXMLElement* getNextElement(NSFXMLElement* childElement, const NSFString& nextTag) const;

        /// <summary>
        /// Gets the number of child elements
        /// </summary>
        /// <returns>The number of child elements.</returns>
        UInt32 getNumberOfChildElements() const; 

        /// <summary>
        /// Gets the parent element.
        /// </summary>
        /// <returns>The child element, or NULL if none exist.</returns>
        NSFXMLElement* getParentElement() const;

        /// <summary>
        /// Gets the first parent element with the specified tag.
        /// </summary>
        /// <param name="parentTag">The parent element tag.</param>
        /// <returns>The parent element, or NULL if none exist.</returns>
        NSFXMLElement* getParentElement(const NSFString& parentTag) const;

        /// <summary>
        ///  Gets the element's tag.
        /// </summary>
        /// <returns>The tag.</returns>
        const NSFString& getTag() const;

        /// <summary>
        ///  Gets the element's text.
        /// </summary>
        /// <returns>The text.</returns>
        const NSFString& getText() const;

        /// <summary>
        /// Loads the element data from a string buffer.  The buffer must be in valid XML format.
        /// </summary>
        /// <returns>True if successful, false otherwise.</returns>
        bool loadBuffer(const NSFString& buffer);

        /// <summary>
        /// Saves the element to the specified stream.
        /// </summary>
        /// <param name="stream">The stream.</param>
        void save(std::ofstream& stream);

        /// <summary>
        /// Sets the element's tag.
        /// </summary>
        /// <param name="tag">The tag.</param>
        void setTag(const NSFString& tag);

        /// <summary>
        /// Sets the element's text.
        /// </summary>
        /// <param name="text">The text.</param>
        void setText(const NSFString& text);

    private:

        std::list<NSFXMLElement*> childElements;
        NSFXMLElement* parentElement;
        NSFString tag;
        NSFString text;

        /// <summary>
        /// Escapes special characters in the specified string.
        /// </summary>
        /// <param name="stringToEscape">The string to escape.</param>
        void escape(NSFString& stringToEscape);

        /// <summary>
        /// Loads the element data from a string.
        /// </summary>
        /// <param name="buffer">The string buffer to load from.</param>
        /// <param name="readPosition">The read position to start from.</param>
        /// <returns>True if successful, false otherwise.</returns>
        /// <remarks>
        /// The data must be in valid XML format.
        /// </remarks>
        bool loadBuffer(const NSFString& buffer, UInt32& readPosition);

        /// <summary>
        /// Parses an element's attributes.
        /// </summary>
        /// <param name="buffer">The string buffer to load from.</param>
        /// <param name="readPosition">The read position to start from.</param>
        /// <returns>True if successful, false otherwise.</returns>
        /// <remarks>
        /// This implementation does not support reading attribute values, therefore
        /// this is a dummy operation that simply looks for the end of the start tag
        /// </remarks>
        bool parseAttributes(const NSFString& buffer, UInt32& readPosition);

        /// <summary>
        /// Parses the element's child elements.
        /// </summary>
        /// <param name="buffer">The string buffer to parse.</param>
        /// <param name="readPosition">The read position to start from.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool parseChildElements(const NSFString& buffer, UInt32& readPosition);

        /// <summary>
        /// Parses the element's end tag.
        /// </summary>
        /// <param name="buffer">The string buffer to parse.</param>
        /// <param name="readPosition">The read position to start from.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool parseEndTag(const NSFString& buffer, UInt32& readPosition);

        /// <summary>
        /// Parses an element entity.
        /// </summary>
        /// <param name="buffer">The string buffer to parse.</param>
        /// <param name="readPosition">The read position to start from.</param>
        /// <returns>True if successful, false otherwise.</returns>
        /// <remarks>
        /// The argument readPosition should be pointing to '&' at beginning of entity
        /// </remarks>
        bool parseEntity(const NSFString& buffer, UInt32& readPosition);

        /// <summary>
        /// Parses an element's starting tag.
        /// </summary>
        /// <param name="buffer">The string buffer to parse.</param>
        /// <param name="readPosition">The read position to start from.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool parseStartTag(const NSFString& buffer, UInt32& readPosition);

        /// <summary>
        /// Parses an element's text.
        /// </summary>
        /// <param name="buffer">The string buffer to parse.</param>
        /// <param name="readPosition">The read position to start from.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool parseText(const NSFString& buffer, UInt32& readPosition);

        /// <summary>
        /// Utility function to replace a character with a string.
        /// </summary>
        /// <param name="stringToModify">The string buffer to modify.</param>
        /// <param name="characterToReplace">The character to replace.</param>
        /// <param name="stringToInsert">The string to replace the character with.</param>
        void replaceCharWithString(NSFString& stringToModify, const NSFString& characterToReplace, const NSFString& stringToInsert);

        /// <summary>
        /// Skips over an element's whitespace.
        /// </summary>
        /// <param name="buffer">The string buffer to parse.</param>
        /// <param name="readPosition">The read position to start from.</param>
        /// <returns>True if successful, false otherwise.</returns>
        bool skipWhitespace(const NSFString& buffer, UInt32& readPosition);

        /// <summary>
        /// Utility function to write a line to the specified ofstream.
        /// </summary>
        /// <param name="string">The string to write out.</param>
        /// <param name="stream">The stream to write to.</param>
        void writeLine(const NSFString& string, std::ofstream& stream);
    };
}

#endif // NSF_XML_ELEMENT_H
