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
#include "convarvariable.h"

#include <sstream>
#include <iomanip>

using namespace std;


ConVarVariable::ConVarVariable (const ConVarAttributes& attributes, ConVar* value) :
   ConVarState (attributes), mValue (value)
{
}

ConVarState* ConVarVariable::clone() const
{
   return new ConVarVariable (mAttributes, mValue);
}

   

ConVar::EConVarType ConVarVariable::getType() const
{
   return ConVar::CVT_VARIABLE;
}



bool ConVarVariable::setInt (int value)
{
   return mValue->setInt (value);
}

bool ConVarVariable::setFloat (float value)
{
   return mValue->setFloat (value);
}

bool ConVarVariable::setString (const string& value)
{
   return mValue->setString (value);
}

bool ConVarVariable::setCharString (const char* value)
{
   return mValue->setCharString (value);
}

bool ConVarVariable::setVector (const Vector3& value)
{
   return mValue->setVector (value);
}

bool ConVarVariable::setCommand (const ConCommand* value)
{
   return mValue->setCommand (value);
}


bool ConVarVariable::setVariable (ConVar* value)
{
   return mValue->setVariable (value);
}




bool ConVarVariable::getInt (int& value) const
{
   return mValue->getInt (value);
}

bool ConVarVariable::getFloat (float& value) const
{
   return mValue->getFloat (value);
}

bool ConVarVariable::getString (string& value) const
{
   return mValue->getString (value);
}

bool ConVarVariable::getCharString (char** value) const
{
   return mValue->getCharString (value);
}

bool ConVarVariable::getCommand (const ConCommand** value) const
{
   return mValue->getCommand (value);
}

bool ConVarVariable::getVector (Vector3& value) const
{
   return mValue->getVector (value);
}

bool ConVarVariable::getVariable (ConVar** value) const
{
   *value = mValue;
   return true;
}



// this method should never be called
string ConVarVariable::dumpValue() const
{
   return "";
}

// this method should never be called
string ConVarVariable::dumpType() const
{
   return "";
}


string ConVarVariable::dump() const
{
   return mValue->dump();
}

string ConVarVariable::dumpWithSignature() const
{
   return mValue->dumpWithSignature();
}

