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
#include "convarint.h"

#include <sstream>
#include <iomanip>

using namespace std;

ConVarInt::ConVarInt(const ConVarAttributes& attributes) :
    ConVarState(attributes)
{
}

ConVarInt::ConVarInt(const ConVarAttributes& attributes, int value) :
    ConVarState(attributes), M_value(value)
{
}

ConVarState*
ConVarInt::clone() const
{
    return new ConVarInt(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarInt::getType() const
{
    return ConVar::S_CVT_INT;
}

bool
ConVarInt::setInt(int value)
{
    if (M_attributes.isConstant())
    {
        return false;
    }

    setValue(value);

    return true;
}

bool
ConVarInt::setVariable(ConVar* value)
{
    int contained_value;

    if (value->getInt(contained_value))
    {
        return setInt(contained_value);
    }
    else
    {
        return false;
    }
}

bool
ConVarInt::getInt(int &value) const
{
    value = getValue();
    return true;
}

bool
ConVarInt::getFloat(TFloat& value) const
{
    int int_value;
    getInt(int_value);
    value = static_cast<TFloat>(int_value);

    return true;
}

bool
ConVarInt::getString(string& value) const
{
    int int_value;
    getInt(int_value);

    stringstream ss;
    ss << int_value;
    value = ss.str();

    return true;
}

bool
ConVarInt::getCharString(char** value) const
{
    *value = new char[strlen(dumpValue().c_str()) + 1];
    strcpy(*value, dumpValue().c_str());

    return true;
}

string
ConVarInt::dumpValue() const
{
    string dump;
    getString(dump);
    return dump;
}

string
ConVarInt::dumpType() const
{
    return "int";
}

void
ConVarInt::setValue(int value)
{
    M_value = value;
    M_attributes.callBack();
}

int
ConVarInt::getValue() const
{
    return M_value;
}
