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
#include "convarlabel.h"

using namespace std;

ConVarLabel::ConVarLabel(const ConVarAttributes& attributes, 
                         const string& value) :
    ConVarState(attributes), M_value(value)
{
}

ConVarState*
ConVarLabel::clone() const
{
    return new ConVarLabel(M_attributes, M_value);
}

ConVar::ConVarType 
ConVarLabel::getType() const
{
    return ConVar::S_CVT_LABEL;
}

bool
ConVarLabel::setLabel(const string& value)
{
    if (M_attributes.isConstant())
    {
        return false;
    }

    setValue(value);
    return true;
}

bool
ConVarLabel::getLabel(std::string& value) const
{
    value = getValue();

    return true;
}

string
ConVarLabel::dumpValue() const
{
    string dump;
    getString(dump);

    return dump;
}

string
ConVarLabel::dumpType() const
{
    return "label";
}

void
ConVarLabel::setValue(const string& value)
{
    M_value = value;
    M_attributes.callBack();
}

string
ConVarLabel::getValue() const
{
    return M_value;
}
