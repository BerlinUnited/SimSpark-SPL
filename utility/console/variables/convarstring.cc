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
#include "convarstring.h"

#include <sstream>
#include <iomanip>

using namespace std;

ConVarString::ConVarString(const ConVarAttributes& attributes) :
    ConVarState(attributes)
{
}

ConVarString::ConVarString(const ConVarAttributes& attributes,
                           const string& value) :
    ConVarState(attributes), M_value(value)
{
}

ConVarState*
ConVarString::clone() const
{
    return new ConVarString(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarString::getType() const
{
    return ConVar::S_CVT_STRING;
}

bool
ConVarString::setInt(int value)
{
    stringstream ss;

    ss << value;

    return setString(ss.str());
}

bool
ConVarString::setFloat(TFloat value)
{
    stringstream ss;

    ss << value;

    return setString(ss.str());
}

bool
ConVarString::setString(const string& value)
{
    if (M_attributes.isConstant())
    {
        return false;
    }

    setValue(value);
    return true;
}

bool
ConVarString::setCharString(const char* value)
{
    return setString(value);
}

bool
ConVarString::setVariable(ConVar* value)
{
    string contained_value;

    if (value->getString(contained_value))
    {
        return setString(contained_value);
    }
    else
    {
        return false;
    }
}

bool
ConVarString::getString(std::string& value) const
{
    value = getValue();

    return true;
}

bool
ConVarString::getCharString(char** value) const
{
    string string_value;
    getString(string_value);

    *value = new char[strlen(string_value.c_str()) + 1];
    strcpy(*value, string_value.c_str());

    return true;
}

string
ConVarString::dumpValue() const
{
    string dump;
    getString(dump);

    return "'" + dump + "'";
}

string
ConVarString::dumpType() const
{
    return "string";
}

void
ConVarString::setValue(const string& value)
{
    M_value = value;
    M_attributes.callBack();
}

string
ConVarString::getValue() const
{
    return M_value;
}
