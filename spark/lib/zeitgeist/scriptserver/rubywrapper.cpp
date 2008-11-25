/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#include "rubywrapper.h"

VALUE zeitgeist::RbFuncallWrap(VALUE arg)
{
    RbArguments &a = *reinterpret_cast<RbArguments*>(arg);
    return rb_funcall2(a.receiver, a.id, a.n, a.argv);
}

VALUE zeitgeist::RbEvalStringWrap(const std::string& str, int& error)
{
  VALUE v = rb_eval_string_protect(str.c_str(),&error);

  if (error)
    {
      RbPrintError();
      return Qnil;
    }

  return v;
}


VALUE zeitgeist::RbEvalStringWrap(const std::string& str)
{
  int error;
  return RbEvalStringWrap(str,error);
}

std::string zeitgeist::RbGetError()
{
  VALUE mes = rb_inspect(rb_gv_get("$!"));
  return RSTRING(mes)->ptr;
}

void zeitgeist::RbPrintError()
{
  std::cout << RbGetError().c_str() << std::endl;
  rb_backtrace();
}
