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
#include "convarattributes.h"
#include "convarcallback.h"

using namespace std;

/*! the static default variable is initialized with the default
    constructor initialization
 */
ConVarAttributes ConVarAttributes::S_default;

ConVarAttributes::ConVarAttributes()
{
    M_name = "";
    M_removable = true;
    M_constant = false;
    M_group = S_CVG_USER;
    M_call_back_object = 0;
}

ConVarAttributes::ConVarAttributes(const ConVarAttributes& attributes)
{
    M_name = attributes.M_name;
    M_help = attributes.M_help;
    M_removable = attributes.M_removable;
    M_constant = attributes.M_constant;
    M_group = attributes.M_group;
    M_call_back_object = attributes.M_call_back_object;
    M_call_back_index = attributes.M_call_back_index;
}

ConVarAttributes 
ConVarAttributes::getDefault()
{
    return S_default;
}

void
ConVarAttributes::setName(std::string name)
{
    M_name = name;
}

std::string 
ConVarAttributes::getName() const
{
    return M_name;
}

void
ConVarAttributes::setRemovable(bool removable)
{
    M_removable = removable;
}

bool 
ConVarAttributes::isRemovable() const
{
    return M_removable;
}

void
ConVarAttributes::setConstant(bool constant)
{
    M_constant = constant;
}

bool 
ConVarAttributes::isConstant() const
{
    return M_constant;
}

void
ConVarAttributes::setGroup(ConVarGroup group)
{
    M_group = group;
}

ConVarAttributes::ConVarGroup 
ConVarAttributes::getGroup() const
{
    return M_group;
}

void
ConVarAttributes::addGroup(ConVarGroup group)
{
    M_group = (ConVarGroup) (M_group | group);
}

void
ConVarAttributes::removeGroup(ConVarGroup group)
{
    M_group = (ConVarGroup) (M_group & group);
}

bool 
ConVarAttributes::isInGroup(ConVarGroup group) const
{
    return (M_group & group);
}

void
ConVarAttributes::setCallBack(ConVarCallBack* call_back_object,
                              int call_back_index)
{
    M_call_back_object = call_back_object;
    M_call_back_index = call_back_index;
}

void
ConVarAttributes::callBack() const
{
    if (M_call_back_object != 0)
    {
        M_call_back_object->callBack(M_call_back_index);
    }
}

void
ConVarAttributes::setHelp(std::string help)
{
    M_help = help;
}

string 
ConVarAttributes::getHelp() const
{
    return M_help;
}

string 
ConVarAttributes::dumpGroup() const
{
    string group;

    if (isInGroup(S_CVG_USER))
    {
        group += "user ";
    }

    if (isInGroup(S_CVG_COMMAND))
    {
        group += "command ";
    }

    if (isInGroup(S_CVG_CON_SET))
    {
        group += "con_set ";
    }

    if (isInGroup(S_CVG_CON_SET_COLOR))
    {
        group += "con_set_col ";
    }

    if (isInGroup(S_CVG_CONST_COLOR))
    {
        group += "con_const_col ";
    }

    return group;
}

string 
ConVarAttributes::dumpRemovable() const
{
    if (isRemovable())
    {
        return "";
    }
    else
    {
        return "unrm";
    }
}

string 
ConVarAttributes::dumpConstant() const
{
    if (isConstant())
    {
        return "const";
    }
    else
    {
        return "";
    }
}
