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


ConVarLabel::ConVarLabel (const ConVarAttributes& attributes, const string& value) :
   ConVarState (attributes), mValue (value)
{
}

ConVarState* ConVarLabel::clone() const
{
   return new ConVarLabel (mAttributes, mValue);
}

   

ConVar::EConVarType ConVarLabel::getType() const
{
   return ConVar::CVT_LABEL;
}



bool ConVarLabel::setLabel (const string& value)
{
    if (mAttributes.isConstant())
    {
        return false;
    }
   
   setValue (value);
   return true;
}

bool ConVarLabel::getLabel (std::string& value) const
{
   value = getValue();
   
   return true;
}


string ConVarLabel::dumpValue() const
{
   string dump;
   getString (dump);

   return dump;
}

string ConVarLabel::dumpType() const
{
   return "label";
}



void ConVarLabel::setValue (string value)
{
   mValue = value;
   mAttributes.CallBack();
}

string ConVarLabel::getValue() const
{
   return mValue;
}
