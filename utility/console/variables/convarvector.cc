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
#include "convarvector.h"

#include <sstream>
#include <iomanip>

using namespace std;

ConVarVector::ConVarVector(const ConVarAttributes& attributes) :
    ConVarState(attributes)
{
}

ConVarVector::ConVarVector(const ConVarAttributes& attributes,
                           const Vector3& value) :
    ConVarState(attributes), M_value(value)
{
}

ConVarState*
ConVarVector::clone() const
{
    return new ConVarVector(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarVector::getType() const
{
    return ConVar::S_CVT_VECTOR;
}

bool
ConVarVector::setVector(const Vector3& value)
{
    if (M_attributes.isConstant())
    {
        return false;
    }

    setValue(value);
    return true;
}

bool
ConVarVector::setVariable(ConVar* value)
{
    Vector3 contained_value;

    if (value->getVector(contained_value))
    {
        return setVector(contained_value);
    }
    else
    {
        return false;
    }
}

bool
ConVarVector::getVector(Vector3& value) const
{
    value = getValue();
    return true;
}

string
ConVarVector::dumpValue() const
{
    stringstream ss;
    Vector3 value = getValue();

    ss << "<" << value.x() << "," << value.y() << "," << value.z() << ">";
    return ss.str();
}

string
ConVarVector::dumpType() const
{
    return "vector";
}

void
ConVarVector::setValue(const Vector3& value)
{
    M_value = value;
    M_attributes.callBack();
}

Vector3
ConVarVector::getValue() const
{
    return M_value;
}
