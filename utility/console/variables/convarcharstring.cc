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
#include "convarcharstring.h"

#include <sstream>
#include <iomanip>

using namespace std;


ConVarCharString::ConVarCharString (const ConVarAttributes& attributes, const char* value) :
   ConVarState (attributes)
{
   // assure that the char* is never empty
   if (value == NULL)
   {
      mValue = new char [1];
      mValue = '\0';
   } else {
      mValue   = new char [strlen (value) + 1];
      strcpy (mValue, value);
   }
}


ConVarCharString::ConVarCharString (const ConVarCharString& conVar)
{
   mAttributes = conVar.mAttributes;
   
   mValue   = new char [strlen (conVar.mValue) + 1];
   strcpy (mValue, conVar.mValue);
}

void ConVarCharString::operator= (const ConVarCharString& conVar)
{
   mAttributes = conVar.mAttributes;
   
   delete[] mValue;

   mValue   = new char [strlen (conVar.mValue) + 1];
   strcpy (mValue, conVar.mValue);
}


ConVarCharString::~ConVarCharString()
{
    delete[] mValue;
}

ConVarState* ConVarCharString::clone() const
{
   return new ConVarCharString (mAttributes, mValue);
}


ConVar::EConVarType ConVarCharString::getType() const
{
   return ConVar::CVT_CHARSTRING;
}



bool ConVarCharString::setInt (int value)
{
   stringstream ss;
   
   ss << value;
   
   return setCharString (ss.str().c_str());
}

bool ConVarCharString::setFloat (float value)
{
   stringstream ss;
   
   ss << value;
   
   return setCharString (ss.str().c_str());
}

bool ConVarCharString::setString (const string& value)
{
   return setCharString (value.c_str());
}

bool ConVarCharString::setCharString (const char* value)
{
    if (mAttributes.isConstant())
    {
        return false;
    }

   setValue (value);
   return true;
}

bool ConVarCharString::setVariable (ConVar* value)
{
   char*   containedValue;
   
   if (! value->getCharString (&containedValue))
   {
      return false;
   }

   bool success = setCharString (containedValue);
   delete containedValue;
   return success;
}




bool ConVarCharString::getString (string& value) const
{
   char* charString;
   getCharString (&charString);
   value = charString;
   
   delete charString;
   
   return true;
}

bool ConVarCharString::getCharString (char** value) const
{
   *value = getValue();
   return true;
}




string ConVarCharString::dumpValue() const
{
   string dump;
   getString (dump);

   stringstream ss;
   ss << "'" << dump << "'";
   return ss.str();
}

string ConVarCharString::dumpType() const
{
   return "char*";
}




void ConVarCharString::setValue (const char* value)
{
   delete[] mValue;
   mValue   = new char [strlen (value) + 1];
   strcpy (mValue, value);
   
   mAttributes.CallBack();
}

char* ConVarCharString::getValue() const
{
   char* value = new char [strlen (mValue) + 1];
   strcpy (value, mValue);

   return value;
}
