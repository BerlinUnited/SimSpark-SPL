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
#include "convarbool.h"

using namespace std;


ConVarBool::ConVarBool (const ConVarAttributes& attributes) 
   : ConVarState (attributes)
{
}

ConVarBool::ConVarBool (const ConVarAttributes& attributes, const bool value) 
   : ConVarState (attributes), mValue (value)
{
}


ConVarState* ConVarBool::clone() const
{
   return new ConVarBool (mAttributes, mValue);
}


ConVar::EConVarType ConVarBool::getType() const
{
   return ConVar::CVT_BOOL;
}



bool ConVarBool::setBool (const bool value)
{
    if (mAttributes.isConstant())
    {
        return false;
    }
   
   setValue (value);

   return true;
}

bool ConVarBool::setVariable (ConVar* value)
{
   bool containedValue;
   
   if (value->getBool (containedValue))
   {
      return setBool (containedValue);
   } else {
      return false;
   }
}



bool ConVarBool::getBool (bool& value) const
{
   value = getValue();
   return true;
}




string ConVarBool::dumpValue() const
{
   if (getValue())
   {
      return "true";
   } else {
      return "false";
   }
}

string ConVarBool::dumpType() const
{
   return "bool";
}




void ConVarBool::setValue (bool value)
{
   mValue = value;
   mAttributes.CallBack();
}

bool ConVarBool::getValue() const
{
   return mValue;
}
