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
#include "convarbool.h"

using namespace std;

ConVarBool::ConVarBool(const ConVarAttributes& attributes) :
    ConVarState(attributes)
{
}

ConVarBool::ConVarBool(const ConVarAttributes& attributes, bool value) :
    ConVarState(attributes), M_value(value)
{
}

ConVarState*
ConVarBool::clone() const
{
    return new ConVarBool(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarBool::getType() const
{
    return ConVar::S_CVT_BOOL;
}

bool ConVarBool::setBool(bool value)
{
    if (M_attributes.isConstant())
    {
        return false;
    }

    setValue(value);

    return true;
}

bool 
ConVarBool::setVariable(ConVar* value)
{
    bool contained_value;

    if (value->getBool(contained_value))
    {
        return setBool(contained_value);
    }
    else
    {
        return false;
    }
}

bool 
ConVarBool::getBool(bool& value) const
{
    value = getValue();
    return true;
}

string 
ConVarBool::dumpValue() const
{
    if (getValue())
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

string 
ConVarBool::dumpType() const
{
    return "bool";
}

void
ConVarBool::setValue(bool value)
{
    M_value = value;
    M_attributes.callBack();
}

bool 
ConVarBool::getValue() const
{
    return M_value;
}
