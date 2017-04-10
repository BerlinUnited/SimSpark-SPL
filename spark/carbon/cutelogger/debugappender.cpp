/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

// Platform dependant printing from:
// Copyright (c) 2010, Razvan Petru
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

// Local
#include "debugappender.h"

// STL
#include <iostream>

#if defined(Q_OS_WIN)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
void output( const QString& message )
{
   OutputDebugStringW(reinterpret_cast<const WCHAR*>(message.utf16()));
   OutputDebugStringW(L"");
}
#elif defined(Q_OS_SYMBIAN)
#include <e32debug.h>
void output( const QString& message )
{
   TPtrC8 symbianMessage(reinterpret_cast<const TUint8*>(qPrintable(message)));
   RDebug::RawPrint(symbianMessage);
}
#elif defined(Q_OS_UNIX)
#include <cstdio>
void output( const QString& message )
{
   fprintf(stderr, "%s", qPrintable(message));
   fflush(stderr);
}
#endif

void DebugAppender::append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                             const char* function, unsigned int source, const QString& message)
{
  output(qPrintable(formattedString(timeStamp, logLevel, file, line, function, source, message)));
}
