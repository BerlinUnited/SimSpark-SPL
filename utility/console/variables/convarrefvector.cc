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
#include "convarrefvector.h"

#include <sstream>
#include <iomanip>

using namespace std;

ConVarRefVector::ConVarRefVector(const ConVarAttributes& attributes, 
                                 Vector3* value) :
    ConVarVector(attributes), M_value(value)
{
}

ConVarState*
ConVarRefVector::clone() const
{
    return new ConVarRefVector(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarRefVector::getType() const
{
    return ConVar::S_CVT_REF_VECTOR;
}

string
ConVarRefVector::dumpType() const
{
    return "vector ref";
}

void
ConVarRefVector::setValue(const Vector3& value)
{
    *M_value = value;
    M_attributes.callBack();
}

Vector3
ConVarRefVector::getValue() const
{
    return *M_value;
}
