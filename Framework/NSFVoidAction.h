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

#ifndef NSF_VOID_ACTION_H
#define NSF_VOID_ACTION_H

#include "NSFDelegateContext.h"
#include <map>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a general purpose action.
    /// </summary>
    /// <typeparam name="ContextType">The context type passed as an argument to the action.</typeparam>
    /// <remarks>
    /// An action is a delegate type used by the North State Framework.
    /// </remarks>
    template<class ContextType> 
    class NSFVoidAction
    {
    public:

        /// <summary>
        /// Destroys the action.
        /// </summary>
        virtual ~NSFVoidAction();

        /// <summary>
        /// Invokes the action.
        /// </summary>
        /// <param name="context">The context passed to the action.</param>
        virtual void operator()(const ContextType& context) = 0;

        /// <summary>
        /// Compares the action to another.
        /// </summary>
        /// <param name="other">The other action to compare against.</param>
        /// <remarks>
        /// Two actions are considered equal if they result in the same function call to the same object, or the same global function call.
        /// </remarks>
        virtual bool operator==(const NSFVoidAction& other) const = 0;

        /// <summary>
        /// Copies the action.
        /// </summary>
        virtual NSFVoidAction* copy() const = 0;
    };

#if (defined WIN32) || (defined WINCE)
    // Compiler bug issues warning 4121 with pointer to member function
#pragma warning( push )
#pragma warning( disable : 4121 )
#endif

    /// <summary>
    /// Represents a member function action.
    /// </summary>
    /// <typeparam name="ObjectType">The class type containing the member function.</typeparam>
    /// <typeparam name="ContextType">The context type passed as an argument to the member function.</typeparam>
    /// <remarks>
    /// An action is a delegate type used by the North State Framework.
    /// This class can be instantiated with either a unary or nullary (argument-less) member function.
    /// This allows nullary delegates to be placed in lists of unary delegates and called without arguments via the function operator.
    /// This feature is convenient for attaching existing nullary methods to action lists, without having to wrap them in action methods.
    /// </remarks>
    template<class ObjectType, class ContextType> 
    class NSFVoidMemberAction : public NSFVoidAction<ContextType>
    {
    public:

        /// <summary>
        /// Creates a member function action.
        /// </summary>
        /// <param name="obj">The object on which the member function is called.</param>
        /// <param name="memFunc">The member function called by the action.</param>
        NSFVoidMemberAction(ObjectType* obj, void (ObjectType::*memFunc)(const ContextType&));


        /// <summary>
        /// Creates a member function action from a nullary member function.
        /// </summary>
        /// <param name="obj">The object on which the member function is called.</param>
        /// <param name="memFunc">The member function called by the action.</param>
        NSFVoidMemberAction(ObjectType* obj, void (ObjectType::*memFunc)());

        virtual void operator()(const ContextType& context);

        virtual bool operator==(const NSFVoidAction<ContextType>& other) const;

        virtual NSFVoidMemberAction* copy() const;

    protected:

        ObjectType* object;
        void (ObjectType::*memberFunction)(const ContextType&);
        void (ObjectType::*nullaryMemberFunction)();
    };

#if (defined WIN32) || (defined WINCE)
#pragma warning( pop )
#endif

    /// <summary>
    /// Represents a global function action.
    /// </summary>
    /// <typeparam name="ContextType">The context type passed as an argument to the global function.</typeparam>
    /// <remarks>
    /// An action is a delegate type used by the North State Framework.
    /// This class can be instantiated with either a unary or nullary (argument-less) global function.
    /// This allows nullary delegates to be placed in lists of unary delegates and called without arguments via the function operator.
    /// This feature is convenient for attaching existing nullary methods to action lists, without having to wrap them in action methods.
    /// </remarks>
    template<class ContextType> 
    class NSFVoidGlobalAction : public NSFVoidAction<ContextType>
    {
    public:

        /// <summary>
        /// Creates a global function action.
        /// </summary>
        /// <param name="globFunc">The global function called by the action.</param>
        NSFVoidGlobalAction(void (*globFunc)(const ContextType&));

        /// <summary>
        /// Creates a global function action from a nullary global function.
        /// </summary>
        /// <param name="globFunc">The global function called by the action.</param>
        NSFVoidGlobalAction(void (*globFunc)());

        virtual void operator()(const ContextType& context);

        virtual bool operator==(const NSFVoidAction<ContextType>& other) const;

        NSFVoidGlobalAction* copy() const;

    protected:

        void (*globalFunction)(const ContextType&);
        void (*nullaryGlobalFunction)();
    };

    /// <summary>
    /// Represents a template method to construct a member action.
    /// </summary>
    /// <remarks>
    /// Template methods shorten the syntax during construction of actions as they can use template argument inference.
    /// </remarks>
    template<class ObjectType, class ContextType> 
    NSFVoidMemberAction<ObjectType, ContextType> NSFAction(ObjectType* obj, void (ObjectType::*memFunc)(const ContextType&))
    {
        return NSFVoidMemberAction<ObjectType, ContextType>(obj, memFunc);
    }

    /// <summary>
    /// Represents a template method to construct a member action.
    /// </summary>
    /// <remarks>
    /// This method is provided for consistency of syntax with member functions taking a ContextType argument.
    /// </remarks>
    template<class ObjectType, class ContextType> 
    NSFVoidMemberAction<ObjectType, ContextType> NSFAction(ObjectType* obj, void (ObjectType::*memFunc)())
    {
        return NSFVoidMemberAction<ObjectType, ContextType>(obj, memFunc);
    }

    /// <summary>
    /// Represents a template method to construct a global action.
    /// </summary>
    /// <remarks>
    /// Template methods shorten the syntax during construction of actions as they can use template argument inference.
    /// </remarks>
    template<class ContextType> 
    NSFVoidGlobalAction<ContextType> NSFAction(void (*globFunc)(const ContextType&))
    {
        return NSFVoidGlobalAction<ContextType>(globFunc);
    }

    /// <summary>
    /// Represents a template method to construct a global action.
    /// </summary>
    /// <remarks>
    /// This method is provided for consistency of syntax with global functions taking a ContextType argument.
    /// </remarks>
    template<class ContextType> 
    NSFVoidGlobalAction<ContextType> NSFAction(void (*globFunc)())
    {
        return NSFVoidGlobalAction<ContextType>(globFunc);
    }


    // Template method definitions

    // NSFVoidAction

    template<class ContextType>  
    NSFVoidAction<ContextType>::~NSFVoidAction()
    {
    }

    // NSFVoidMemberAction

    template<class ObjectType, class ContextType>  
    NSFVoidMemberAction<ObjectType, ContextType>::NSFVoidMemberAction(ObjectType* obj, void (ObjectType::*memFunc)(const ContextType&))
        : object(obj), memberFunction(memFunc), nullaryMemberFunction(NULL)
    {
        if ((object == NULL) || (memberFunction == NULL))
        {
            throw std::runtime_error("Invalid NSFVoidMemberAction");
        }
    }

    template<class ObjectType, class ContextType>  
    NSFVoidMemberAction<ObjectType, ContextType>::NSFVoidMemberAction(ObjectType* obj, void (ObjectType::*memFunc)())
        : object(obj), memberFunction(NULL), nullaryMemberFunction(memFunc)
    {
        if ((object == NULL) || (nullaryMemberFunction == NULL))
        {
            throw std::runtime_error("Invalid NSFVoidMemberAction");
        }
    }

    template<class ObjectType, class ContextType>  
    void NSFVoidMemberAction<ObjectType, ContextType>::operator()(const ContextType& context)
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
    bool NSFVoidMemberAction<ObjectType, ContextType>::operator==(const NSFVoidAction<ContextType>& other) const
    {
        const NSFVoidMemberAction<ObjectType, ContextType>* otherDelegate = dynamic_cast<const NSFVoidMemberAction<ObjectType, ContextType>*>(&other);
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
    NSFVoidMemberAction<ObjectType, ContextType>* NSFVoidMemberAction<ObjectType, ContextType>::copy() const
    {
        if (memberFunction != NULL)
        {
            return new NSFVoidMemberAction(object, memberFunction);
        }
        else
        {
            return new NSFVoidMemberAction(object, nullaryMemberFunction);
        }
    }

    // NSFVoidGlobalAction

    template<class ContextType>  
    NSFVoidGlobalAction<ContextType>::NSFVoidGlobalAction(void (*globFunc)(const ContextType&))
        : globalFunction(globFunc), nullaryGlobalFunction(NULL)
    {
        if (globalFunction == NULL)
        {
            throw std::runtime_error("Invalid NSFVoidGlobalAction");
        }
    }

    template<class ContextType>  
    NSFVoidGlobalAction<ContextType>::NSFVoidGlobalAction(void (*globFunc)())
        : globalFunction(NULL), nullaryGlobalFunction(globFunc)
    {
        if (nullaryGlobalFunction == NULL)
        {
            throw std::runtime_error("Invalid NSFVoidGlobalAction");
        }
    }

    template<class ContextType>  
    void NSFVoidGlobalAction<ContextType>::operator()(const ContextType& context)
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
    bool NSFVoidGlobalAction<ContextType>::operator==(const NSFVoidAction<ContextType>& other) const
    {
        const NSFVoidGlobalAction<ContextType>* otherDelegate = dynamic_cast<const NSFVoidGlobalAction<ContextType>*>(&other);
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
    NSFVoidGlobalAction<ContextType>* NSFVoidGlobalAction<ContextType>::copy() const
    {
        if (globalFunction != NULL)
        {
            return new NSFVoidGlobalAction(globalFunction);
        }
        else
        {
            return new NSFVoidGlobalAction(nullaryGlobalFunction);
        }
    }
}

#endif // NSF_VOID_ACTION_H
