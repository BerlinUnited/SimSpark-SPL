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
#include "convarrefcharstring.h"

#include <sstream>
#include <iomanip>

using namespace std;

ConVarRefCharString::ConVarRefCharString(const ConVarAttributes& attributes, 
                                         char** value) :
    ConVarCharString(attributes, 0), M_value(value)
{
}

ConVarState*
ConVarRefCharString::clone() const
{
    return new ConVarRefCharString(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarRefCharString::getType() const
{
    return ConVar::S_CVT_REF_CHARSTRING;
}

string
ConVarRefCharString::dumpType() const
{
    return "char* ref";
}

void
ConVarRefCharString::setValue(const char* value)
{
    delete[] * M_value;
    *M_value = new char[strlen(value) + 1];
    strcpy(*M_value, value);

    M_attributes.callBack();
}

char*
ConVarRefCharString::getValue() const
{
    char *value = new char[strlen(*M_value) + 1];
    strcpy(value, *M_value);

    return value;
}
