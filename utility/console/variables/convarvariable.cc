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
#include "convarvariable.h"

#include <sstream>
#include <iomanip>

using namespace std;

ConVarVariable::ConVarVariable(const ConVarAttributes& attributes, 
                               ConVar* value) :
    ConVarState(attributes), M_value(value)
{
}

ConVarState*
ConVarVariable::clone() const
{
    return new ConVarVariable(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarVariable::getType() const
{
    return ConVar::S_CVT_VARIABLE;
}

bool
ConVarVariable::setInt(int value)
{
    return M_value->setInt(value);
}

bool
ConVarVariable::setFloat(TFloat value)
{
    return M_value->setFloat(value);
}

bool
ConVarVariable::setString(const string& value)
{
    return M_value->setString(value);
}

bool
ConVarVariable::setCharString(const char* value)
{
    return M_value->setCharString(value);
}

bool
ConVarVariable::setVector(const Vector3& value)
{
    return M_value->setVector(value);
}

bool
ConVarVariable::setCommand(const ConCommand* value)
{
    return M_value->setCommand(value);
}

bool
ConVarVariable::setVariable(ConVar* value)
{
    return M_value->setVariable(value);
}

bool
ConVarVariable::getInt(int& value) const
{
    return M_value->getInt(value);
}

bool
ConVarVariable::getFloat(TFloat& value) const
{
    return M_value->getFloat(value);
}

bool
ConVarVariable::getString(string& value) const
{
    return M_value->getString(value);
}

bool
ConVarVariable::getCharString(char** value) const
{
    return M_value->getCharString(value);
}

bool
ConVarVariable::getCommand(const ConCommand** value) const
{
    return M_value->getCommand(value);
}

bool
ConVarVariable::getVector(Vector3& value) const
{
    return M_value->getVector(value);
}

bool
ConVarVariable::getVariable(ConVar** value) const
{
    *value = M_value;
    return true;
}

// this method should never be called
string
ConVarVariable::dumpValue() const
{
    return "";
}

// this method should never be called
string
ConVarVariable::dumpType() const
{
    return "";
}

string
ConVarVariable::dump() const
{
    return M_value->dump();
}

string
ConVarVariable::dumpWithSignature() const
{
    return M_value->dumpWithSignature();
}
