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
#include <sstream>
#include <iomanip>

#include "convarfloat.h"

using namespace std;

ConVarFloat::ConVarFloat(const ConVarAttributes& attributes) :
    ConVarState(attributes)
{
}

ConVarFloat::ConVarFloat(const ConVarAttributes& attributes, TFloat value) :
    ConVarState(attributes), M_value(value)
{
}

ConVarState*
ConVarFloat::clone() const
{
    return new ConVarFloat(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarFloat::getType() const
{
    return ConVar::S_CVT_FLOAT;
}

bool 
ConVarFloat::setInt(int value)
{
    return setFloat(static_cast < TFloat > (value));
}

bool 
ConVarFloat::setFloat(TFloat value)
{
    if (M_attributes.isConstant())
    {
        return false;
    }

    setValue(value);
    return true;
}

bool 
ConVarFloat::setVariable(ConVar* value)
{
    TFloat contained_value;

    if (value->getFloat(contained_value))
    {
        return setFloat(contained_value);
    }
    else
    {
        return false;
    }
}

bool 
ConVarFloat::getFloat(TFloat& value) const
{
    value = getValue();
    return true;
}

bool 
ConVarFloat::getString(string& value) const
{
    TFloat float_value;
    getFloat(float_value);

    stringstream ss;
    ss << float_value;
    value = ss.str();

    return true;
}

bool 
ConVarFloat::getCharString(char** value) const
{
    *value = new char[strlen(dumpValue().c_str()) + 1];
    strcpy(*value, dumpValue().c_str());

    return true;
}

string 
ConVarFloat::dumpValue() const
{
    string dump;
    getString(dump);
    return dump;
}

string 
ConVarFloat::dumpType() const
{
    return "float";
}

void
ConVarFloat::setValue(TFloat value)
{
    M_value = value;
    M_attributes.callBack();
}

TFloat 
ConVarFloat::getValue() const
{
    return M_value;
}
