/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: baseparser.cpp,v 1.1.2.1 2003/12/23 11:56:49 rollmark Exp $

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
#include "baseparser.h"

/*
  The following declarations are here to force the compiler to
  generate a type_info object for the template classes
  boost::holder<>. Several plugins, e.g. PerfectVisionPerceptor and
  SexpParser rely on the runtime type information to generate and
  parse TParameterLists.

  If these declarations are omitted the linker does not resolve the
  separately generated type_info classes for each module and a
  comparison between otherwise identical types fails.

  Strictly it would be sufficient to typedef two instances of the
  boost::holder<> class. But as this class is an implementation detail
  of boost I chose the more portable way and went with it's public
  wrapper boost:any.

  The following declarations are unfortunately needed for all
  structured types used inside a TParameterList. Currently this only
  is std::string and the TParameterList itself to allow nesting of
  parameter lists.
*/

namespace
{
  const oxygen::BaseParser::TParameterList paramList;
  const boost::any anyDummyParamList(paramList);

  const std::string str;
  const boost::any anyStr(str);
};


