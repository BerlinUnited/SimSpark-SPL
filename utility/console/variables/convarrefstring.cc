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
#include "convarrefstring.h"

#include <sstream>
#include <iomanip>

using namespace std;

ConVarRefString::ConVarRefString(const ConVarAttributes& attributes, 
                                 string* value) :
    ConVarString(attributes), M_value(value)
{
}

ConVarState*
ConVarRefString::clone() const
{
    return new ConVarRefString(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarRefString::getType() const
{
    return ConVar::S_CVT_REF_STRING;
}

string
ConVarRefString::dumpType() const
{
    return "string ref";
}

void
ConVarRefString::setValue(string value)
{
    *M_value = value;
    M_attributes.callBack();
}

string
ConVarRefString::getValue() const
{
    return *M_value;
}
