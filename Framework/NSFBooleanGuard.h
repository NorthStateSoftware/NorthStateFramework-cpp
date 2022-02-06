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

#ifndef NSF_BOOLEAN_GUARDS_H
#define NSF_BOOLEAN_GUARDS_H

#include "NSFDelegateContext.h"
#include <map>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a general purpose guard.
    /// </summary>
    /// <typeparam name="ContextType">The context type passed as an argument to the guard.</typeparam>
    /// <remarks>
    /// An guard is a delegate type used by the North State Framework.
    /// </remarks>
    template<class ContextType> 
    class NSFBoolGuard
    {
    public:

        /// <summary>
        /// Destroys the guard.
        /// </summary>
        virtual ~NSFBoolGuard();

        /// <summary>
        /// Invokes the guard.
        /// </summary>
        /// <param name="context">The context passed to the guard.</param>
        virtual bool operator()(const ContextType& context) = 0;

        /// <summary>
        /// Compares the guard to another.
        /// </summary>
        /// <param name="other">The other guard to compare against.</param>
        /// <remarks>
        /// Two guard are considered equal if they result in the same function call to the same object, or the same global function call.
        /// </remarks>
        virtual bool operator==(const NSFBoolGuard& other) const = 0;

        /// <summary>
        /// Copies the guard.
        /// </summary>
        virtual NSFBoolGuard* copy() const = 0;
    };

    /// <summary>
    /// Represents a member function guard.
    /// </summary>
    /// <typeparam name="ObjectType">The class type containing the member function.</typeparam>
    /// <typeparam name="ContextType">The context type passed as an argument to the member function.</typeparam>
    /// <remarks>
    /// An guard is a delegate type used by the North State Framework.
    /// This class can be instantiated with either a unary or nullary (argument-less) member function.
    /// This allows nullary delegates to be placed in lists of unary delegates and called without arguments via the function operator.
    /// This feature is convenient for attaching existing nullary methods to guard lists, without having to wrap them in guard methods.
    /// </remarks>
    template<class ObjectType, class ContextType> 
    class NSFBoolMemberGuard : public NSFBoolGuard<ContextType>
    {
    public:

        /// <summary>
        /// Creates a member function guard.
        /// </summary>
        /// <param name="obj">The object on which the member function is called.</param>
        /// <param name="memFunc">The member function called by the action.</param>
        NSFBoolMemberGuard(ObjectType* obj, bool (ObjectType::*memFunc)(const ContextType&));


        /// <summary>
        /// Creates a member function guard from a nullary member function.
        /// </summary>
        /// <param name="obj">The object on which the member function is called.</param>
        /// <param name="memFunc">The member function called by the action.</param>
        NSFBoolMemberGuard(ObjectType* obj, bool (ObjectType::*memFunc)());


        virtual bool operator()(const ContextType& context);

        virtual bool operator==(const NSFBoolGuard<ContextType>& other) const;

        virtual NSFBoolMemberGuard* copy() const;

    protected:

        ObjectType* object;
        bool (ObjectType::*memberFunction)(const ContextType&);
        bool (ObjectType::*nullaryMemberFunction)();
    };

    /// <summary>
    /// Represents a global function guard.
    /// </summary>
    /// <typeparam name="ContextType">The context type passed as an argument to the global function.</typeparam>
    /// <remarks>
    /// An guard is a delegate type used by the North State Framework.
    /// This class can be instantiated with either a unary or nullary (argument-less) global function.
    /// This allows nullary delegates to be placed in lists of unary delegates and called without arguments via the function operator.
    /// This feature is convenient for attaching existing nullary methods to guard lists, without having to wrap them in guard methods.
    /// </remarks>
    template<class ContextType> 
    class NSFBoolGlobalGuard : public NSFBoolGuard<ContextType>
    {
    public:

        /// <summary>
        /// Creates a global function guard.
        /// </summary>
        /// <param name="globFunc">The global function called by the guard.</param>
        NSFBoolGlobalGuard(bool (*globFunc)(const ContextType&));


        /// <summary>
        /// Creates a global function guard from a nullary global function.
        /// </summary>
        /// <param name="globFunc">The global function called by the action.</param>
        NSFBoolGlobalGuard(bool (*globFunc)());


        virtual bool operator()(const ContextType& context);

        virtual bool operator==(const NSFBoolGuard<ContextType>& other) const;

        virtual NSFBoolGlobalGuard* copy() const;

    protected:

        bool (*globalFunction)(ContextType);
        bool (*nullaryGlobalFunction)();
    };

    /// <summary>
    /// Represents a template method to construct a member guard.
    /// </summary>
    /// <remarks>
    /// Template methods shorten the syntax during construction of guards as they can use template argument inference.
    /// </remarks>
    template<class ObjectType, class ContextType> 
    NSFBoolMemberGuard<ObjectType, ContextType> NSFGuard(ObjectType* obj, bool (ObjectType::*memFunc)(const ContextType&))
    {
        return NSFBoolMemberGuard<ObjectType, ContextType>(obj, memFunc);
    }

    /// <summary>
    /// Represents a template method to construct a member guard.
    /// </summary>
    /// <remarks>
    /// This method is provided for consistency of syntax with member functions taking a ContextType argument.
    /// </remarks>
    template<class ObjectType, class ContextType> 
    NSFBoolMemberGuard<ObjectType, ContextType> NSFGuard(ObjectType* obj, bool (ObjectType::*memFunc)())
    {
        return NSFBoolMemberGuard<ObjectType, ContextType>(obj, memFunc);
    }

    /// <summary>
    /// Represents a template method to construct a global guard.
    /// </summary>
    /// <remarks>
    /// Template methods shorten the syntax during construction of guards as they can use template argument inference.
    /// </remarks>
    template<class ContextType> NSFBoolGlobalGuard<ContextType> 
    NSFGuard(bool (*globFunc)(const ContextType&))
    {
        return NSFBoolGlobalGuard<ContextType>(globFunc);
    }

    /// <summary>
    /// Represents a template method to construct a global guard.
    /// </summary>
    /// <remarks>
    /// This method is provided for consistency of syntax with global functions taking a ContextType argument.
    /// </remarks>
    template<class ContextType> NSFBoolGlobalGuard<ContextType> 
    NSFGuard(bool (*globFunc)())
    {
        return NSFBoolGlobalGuard<ContextType>(globFunc);
    }

    // Template method definitions

    // NSFBoolGuard

    template<class ContextType>  
    NSFBoolGuard<ContextType>::~NSFBoolGuard()
    {
    }

    // NSFBoolMemberGuard

    template<class ObjectType, class ContextType>  
    NSFBoolMemberGuard<ObjectType, ContextType>::NSFBoolMemberGuard(ObjectType* obj, bool (ObjectType::*memFunc)(const ContextType&))
        : object(obj), memberFunction(memFunc), nullaryMemberFunction(NULL)
    {
        if ((object == NULL) || (memberFunction == NULL))
        {
            throw std::runtime_error("Invalid NSFBoolMemberGuard");
        }
    }

    template<class ObjectType, class ContextType>  
    NSFBoolMemberGuard<ObjectType, ContextType>::NSFBoolMemberGuard(ObjectType* obj, bool (ObjectType::*memFunc)())
        : object(obj), memberFunction(NULL), nullaryMemberFunction(memFunc)
    {
        if ((object == NULL) || (nullaryMemberFunction == NULL))
        {
            throw std::runtime_error("Invalid NSFBoolMemberGuard");
        }
    }

    template<class ObjectType, class ContextType>  
    bool NSFBoolMemberGuard<ObjectType, ContextType>::operator()(const ContextType& context)
    {
        if (memberFunction != NULL)
        {
            return (object->*memberFunction)(context);
        }
        else
        {
            return (object->*nullaryMemberFunction)();
        }
    }

    template<class ObjectType, class ContextType>  
    bool NSFBoolMemberGuard<ObjectType, ContextType>::operator==(const NSFBoolGuard<ContextType>& other) const
    {
        const NSFBoolMemberGuard<ObjectType, ContextType>* otherDelegate = dynamic_cast<const NSFBoolMemberGuard<ObjectType, ContextType>*>(&other);
        if (otherDelegate != NULL)
        {
            return ((object == otherDelegate->object) && (memberFunction == otherDelegate->memberFunction) && (nullaryMemberFunction == otherDelegate->nullaryMemberFunction));
        }
        else
        {
            return false;
        }
    }

    template<class ObjectType, class ContextType>  
    NSFBoolMemberGuard<ObjectType, ContextType>* NSFBoolMemberGuard<ObjectType, ContextType>::copy() const
    {
        if (memberFunction != NULL)
        {
            return new NSFBoolMemberGuard(object, memberFunction);
        }
        else
        {
            return new NSFBoolMemberGuard(object, nullaryMemberFunction);
        }
    }

    // NSFBoolGlobalGuard

    template<class ContextType>  
    NSFBoolGlobalGuard<ContextType>::NSFBoolGlobalGuard(bool (*globFunc)(const ContextType&))
        : globalFunction(globFunc), nullaryGlobalFunction(NULL)
    {
        if (globalFunction == NULL)
        {
            throw std::runtime_error("Invalid NSFBoolGlobalGuard");
        }
    }

    template<class ContextType>  
    NSFBoolGlobalGuard<ContextType>::NSFBoolGlobalGuard(bool (*globFunc)())
        : globalFunction(NULL), nullaryGlobalFunction(globFunc)
    {
        if (nullaryGlobalFunction == NULL)
        {
            throw std::runtime_error("Invalid NSFBoolGlobalGuard");
        }
    }

    template<class ContextType>  
    bool NSFBoolGlobalGuard<ContextType>::operator()(const ContextType& context)
    {
        if (globalFunction != NULL)
        {
            return (*globalFunction)(context);
        }
        else
        {
            return (*nullaryGlobalFunction)();
        }
    }

    template<class ContextType>  
    bool NSFBoolGlobalGuard<ContextType>::operator==(const NSFBoolGuard<ContextType>& other) const
    {
        const NSFBoolGlobalGuard<ContextType>* otherDelegate = dynamic_cast<const NSFBoolGlobalGuard<ContextType>*>(&other);
        if (otherDelegate != NULL)
        {
            return ((globalFunction == otherDelegate->globalFunction) && (nullaryGlobalFunction == otherDelegate->nullaryGlobalFunction));
        }
        else
        {
            return false;
        }
    }

    template<class ContextType>  
    NSFBoolGlobalGuard<ContextType>* NSFBoolGlobalGuard<ContextType>::copy() const
    {
        if (globalFunction != NULL)
        {
            return new NSFBoolGlobalGuard(globalFunction);
        }
        else
        {
            return new NSFBoolGlobalGuard(nullaryGlobalFunction);
        }
    }
}

#endif // NSF_BOOLEAN_GUARDS_H
