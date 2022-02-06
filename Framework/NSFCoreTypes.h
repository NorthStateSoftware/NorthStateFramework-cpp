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

#ifndef NSF_CORE_TYPES_H
#define NSF_CORE_TYPES_H

#include "NSFCustomConfig.h"

#include <algorithm>
#include <list>
#include <string>
#include <sstream>

#ifndef NULL
#define NULL  0
#endif // NULL

namespace NorthStateFramework
{
#if defined NSF_DEFINE_INTEGER_TYPES

    typedef char Int8;
    typedef short Int16;
    typedef int Int32;
    typedef long long Int64;

    typedef unsigned char UInt8;
    typedef unsigned short UInt16;
    typedef unsigned int UInt32;
    typedef unsigned long long UInt64;

#endif // NSF_DEFINE_INTEGER_TYPES

    typedef Int64 NSFId;
    typedef std::string NSFString;
    typedef Int64 NSFTime;

    const Int32 MilliSecondsPerSecond = 1000;
    const Int32 MicroSecondsPerSecond = 1000000;
    const Int32 NanoSecondsPerSecond = 1000000000;
    const Int32 MicroSecondsPerMilliSecond = 1000;
    const Int32 NanoSecondsPerMilliSecond = 1000000;

    template<class T> inline NSFString toString(const T& value)
    {
        std::stringstream stringStream;
        stringStream << value;
        return stringStream.str();
    }

    inline const NSFString& emptyString()
    {
        static NSFString theEmptyString("");
        return theEmptyString;
    }
}

#endif // NSF_CORE_TYPES_H

