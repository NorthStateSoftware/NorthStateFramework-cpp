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

#ifndef NSF_CORE_TYPES_H
#define NSF_CORE_TYPES_H

#include "NSFCustomConfig.h"

#include <algorithm>
#include <list>
#include <string>
#include <sstream>

#ifndef NULL
#define NULL  0
#endif //NULL

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

    const NSFTime MilliSecondsPerSecond = 1000;
    const NSFTime MicroSecondsPerSecond = 1000000;
    const NSFTime NanoSecondsPerSecond = 1000000000;
    const NSFTime MicroSecondsPerMilliSecond = 1000;
    const NSFTime NanoSecondsPerMilliSecond = 1000000;

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

