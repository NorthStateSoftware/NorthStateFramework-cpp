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

