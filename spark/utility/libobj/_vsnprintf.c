/* -*- mode: c; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: _vsnprintf.c,v 1.1 2003/11/08 22:41:42 fruit Exp $

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

/* this is an implementation of _vsnprintf as used by windows.
   the devil library makes use of _vsnprintf, so linking against libIL
   fails if no _vsnprintf exists.
*/

#include <stdarg.h>
#include <stdio.h>

int
_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
        return vsnprintf(str, size, format, ap);
}
