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
#include "convarint.h"

#include <sstream>
#include <iomanip>

using namespace std;


ConVarInt::ConVarInt (const ConVarAttributes& attributes) :
ConVarState (attributes)
{
}

ConVarInt::ConVarInt (const ConVarAttributes& attributes, int value) :
   ConVarState (attributes), mValue (value)
{
}

ConVarState* ConVarInt::clone() const
{
   return new ConVarInt (mAttributes, mValue);
}

   
ConVar::EConVarType ConVarInt::getType() const
{
   return ConVar::CVT_INT;
}



bool ConVarInt::setInt (int value)
{
    if (mAttributes.isConstant())
    {
        return false;
    }
   
   setValue (value);

   return true;
}

bool ConVarInt::setVariable (ConVar* value)
{
   int containedValue;
   
   if (value->getInt (containedValue))
   {
      return setInt (containedValue);
   } else {
      return false;
   }
}



bool ConVarInt::getInt (int& value) const
{
   value = getValue();
   return true;
}

bool ConVarInt::getFloat (float& value) const
{
   int intValue;
   getInt (intValue);
   value = float (intValue);

   return true;
}

bool ConVarInt::getString (string& value) const
{
   int intValue;
   getInt (intValue);
   
   stringstream ss;
   ss << intValue;
   value = ss.str();

   return true;
}

bool ConVarInt::getCharString (char** value) const
{
   *value = new char [strlen (dumpValue().c_str()) + 1];
   strcpy (*value, dumpValue().c_str());
   
   return true;
}



string ConVarInt::dumpValue() const
{
   string dump;
   getString (dump);
   return dump;
}

string ConVarInt::dumpType() const
{
   return "int";
}




void ConVarInt::setValue (int value)
{
   mValue = value;
   mAttributes.CallBack();
}

int ConVarInt::getValue() const
{
   return mValue;
}
