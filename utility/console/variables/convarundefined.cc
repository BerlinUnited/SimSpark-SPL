/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "convarundefined.h"


#include <sstream>
#include <iomanip>

using namespace std;


ConVarUndefined::ConVarUndefined (const ConVarAttributes& attributes) :
   ConVarState (attributes)
{
}

ConVarState* ConVarUndefined::clone() const
{
   return new ConVarUndefined (mAttributes);
}

   
ConVar::EConVarType ConVarUndefined::getType() const
{
   return ConVar::CVT_UNDEFINED;
}




string ConVarUndefined::dumpValue() const
{
   return "";
}

string ConVarUndefined::dumpType() const
{
   return "Undefined";
}
