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
#include "convarcommand.h"

#include "../commands/concommand.h"

#include <sstream>
#include <iomanip>

using namespace std;



ConVarCommand::ConVarCommand (const ConVarAttributes& attributes, const ConCommand* value) :
   ConVarState (attributes), mValue (value)
{
}

ConVarState* ConVarCommand::clone() const
{
   return new ConVarCommand (mAttributes, mValue);
}
   
   
ConVar::EConVarType ConVarCommand::getType() const
{
   return ConVar::CVT_COMMAND;
}



bool ConVarCommand::setCommand (const ConCommand* value)
{
   if (mAttributes.isConstant())
   {
      return false;
   }
   
   setValue (value);
   return true;
}



bool ConVarCommand::setVariable (ConVar* value)
{
   const ConCommand* containedValue;
   
   if (value->getCommand (&containedValue))
   {
      return setCommand (containedValue);
   } else {
      return false;
   }
}

bool ConVarCommand::getCommand (const ConCommand** value) const
{
   *value = getValue();
   return true;
}




string ConVarCommand::dumpValue() const
{
   return mValue->getUsage();
}

string ConVarCommand::dumpType() const
{
   return "command";
}



void ConVarCommand::setValue (const ConCommand* value)
{
   mValue = value;
   mAttributes.CallBack();
}

const ConCommand* ConVarCommand::getValue() const
{
   return mValue;
}
