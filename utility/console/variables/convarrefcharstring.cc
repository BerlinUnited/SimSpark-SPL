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


ConVarRefCharString::ConVarRefCharString (const ConVarAttributes& attributes, char** value) :
   ConVarCharString (attributes, NULL), mValue (value)
{
}

ConVarState* ConVarRefCharString::clone() const
{
   return new ConVarRefCharString (mAttributes, mValue);
}

   

ConVar::EConVarType ConVarRefCharString::getType() const
{
   return ConVar::CVT_REF_CHARSTRING;
}




string ConVarRefCharString::dumpType() const
{
   return "char* ref";
}



void ConVarRefCharString::setValue (const char* value)
{
   delete[] *mValue;
   *mValue   = new char [strlen (value) + 1];
   strcpy (*mValue, value);
   
   mAttributes.CallBack();
}

char* ConVarRefCharString::getValue() const
{
   char* value = new char [strlen (*mValue) + 1];
   strcpy (value, *mValue);
   
   return value;
}
