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
#include "convarfloat.h"

#include <sstream>
#include <iomanip>

using namespace std;


ConVarFloat::ConVarFloat (const ConVarAttributes& attributes) :
ConVarState (attributes)
{
}

ConVarFloat::ConVarFloat (const ConVarAttributes& attributes, float value) :
   ConVarState (attributes), mValue (value)
{
}

ConVarState* ConVarFloat::clone() const
{
   return new ConVarFloat (mAttributes, mValue);
}

   
ConVar::EConVarType ConVarFloat::getType() const
{
   return ConVar::CVT_FLOAT;
}



bool ConVarFloat::setInt (int value)
{
   return setFloat (float (value));
}


bool ConVarFloat::setFloat (float value)
{
    if (mAttributes.isConstant())
    {
        return false;
    }

   setValue (value);
   return true;
}

bool ConVarFloat::setVariable (ConVar* value)
{
   float containedValue;
   
   if (value->getFloat (containedValue))
   {
      return setFloat (containedValue);
   } else {
      return false;
   }
}



bool ConVarFloat::getFloat (float& value) const
{
   value = getValue();
   return true;
}

bool ConVarFloat::getString (string& value) const
{
   float floatValue;
   getFloat (floatValue);
   
   stringstream ss;
   ss << floatValue;
   value = ss.str();
   
   return true;
}

bool ConVarFloat::getCharString (char** value) const
{
   *value = new char [strlen (dumpValue().c_str()) + 1];
   strcpy (*value, dumpValue().c_str());
   
   return true;
}




string ConVarFloat::dumpValue() const
{
   string dump;
   getString (dump);
   return dump;
}

string ConVarFloat::dumpType() const
{
   return "float";
}



void ConVarFloat::setValue (float value)
{
   mValue = value;
   mAttributes.CallBack();
}

float ConVarFloat::getValue() const
{
   return mValue;
}
