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

#ifndef NSF_CUSTOM_CONFIG_H
#define NSF_CUSTOM_CONFIG_H


// This switch allows applications to control when the first threads are
// started.  Comment out this line to keep the timer and trace log from
// auto starting during program initialization. If this line is commented
// out, applications must call NSFEnvironment::start() before using the
// framework classes.
#define NSF_AUTO_START

// This switch allows applications to define their own types for the core
// integer types: Int8, UInt8, Int16, UInt16, Int32, UInt32, Int64, UInt64.
// Comment out this line to keep NSF from defining these core types. If
// this line is commented out, applications must provide type definitions
// for these types as part of this file, either directly or indirectly.
#define NSF_DEFINE_INTEGER_TYPES

// This switch allows applications to define which operating system to
// target. Comment out all but one of these lines to select the operating
// system, or use a compilation constant.
#define NSF_OS_WIN32
//#define NSF_OS_WINCE
//#define NSF_OS_ECOS
//#define NSF_OS_POSIX


#endif // NSF_CUSTOM_CONFIG_H

