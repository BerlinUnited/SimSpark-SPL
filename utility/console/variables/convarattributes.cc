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
ConVarAttributes ConVarAttributes::sDefault;

ConVarAttributes::ConVarAttributes()
{
    mName = "";
    mRemovable  = true;
    mConstant = false;
    mGroup = eCVG_USER;
    mCallBackObject = NULL;
}

ConVarAttributes::ConVarAttributes(const ConVarAttributes& attributes)
{
    mName = attributes.mName;
    mHelp = attributes.mHelp;
    mRemovable = attributes.mRemovable;
    mConstant = attributes.mConstant;
    mGroup = attributes.mGroup;
    mCallBackObject = attributes.mCallBackObject;
    mCallBackIndex = attributes.mCallBackIndex;
}


ConVarAttributes 
ConVarAttributes::getDefault()
{
    return sDefault;
}

void 
ConVarAttributes::setName(std::string name)
{
    mName = name;
}

std::string 
ConVarAttributes::getName() const
{
    return mName;
}


void 
ConVarAttributes::setRemovable(bool removable)
{
    mRemovable = removable;
}

bool 
ConVarAttributes::isRemovable() const
{
    return mRemovable;
}

void 
ConVarAttributes::setConstant(bool constant)
{
    mConstant = constant;
}

bool 
ConVarAttributes::isConstant() const
{
    return mConstant;
}

void 
ConVarAttributes::setGroup(EConVarGroup group)
{
    mGroup = group;
}

ConVarAttributes::EConVarGroup 
ConVarAttributes::getGroup() const
{
    return mGroup;
}

void 
ConVarAttributes::addGroup(EConVarGroup group)
{
    mGroup = (EConVarGroup) (mGroup | group);
}

void 
ConVarAttributes::removeGroup(EConVarGroup group)
{
    mGroup =(EConVarGroup) (mGroup & group);
}

bool 
ConVarAttributes::isInGroup(EConVarGroup group) const
{
    return (mGroup & group);
}


void
ConVarAttributes::setCallBack(ConVarCallBack* callBackObject, 
			      int callBackIndex)
{
    mCallBackObject = callBackObject;
    mCallBackIndex = callBackIndex;
}

void 
ConVarAttributes::callBack() const
{
    if (mCallBackObject != NULL)
    {
        mCallBackObject->callBack(mCallBackIndex);
    }
}


void 
ConVarAttributes::setHelp(std::string help)
{
    mHelp = help;
}

string 
ConVarAttributes::getHelp() const
{
    return mHelp;
}




string 
ConVarAttributes::dumpGroup() const
{
    string group;

    if (isInGroup(eCVG_USER))
    {
        group += "user ";
    }

    if (isInGroup(eCVG_COMMAND))
    {
        group += "command ";
    }

    if (isInGroup(eCVG_CON_SET))
    {
        group += "con_set ";
    }

    if (isInGroup(eCVG_CON_SET_COLOR))
    {
        group += "con_set_col ";
    }

    if (isInGroup(eCVG_CONST_COLOR))
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
    } else {
        return "unrm";
    } 
}

string 
ConVarAttributes::dumpConstant() const
{
    if (isConstant())
    {
        return "const";
    } else {
        return "";
    }
}
