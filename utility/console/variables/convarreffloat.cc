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
#include "convarreffloat.h"

#include <sstream>
#include <iomanip>

using namespace std;


ConVarRefFloat::ConVarRefFloat (const ConVarAttributes& attributes, float*   value) :
   ConVarFloat (attributes), mValue (value)
{
}

ConVarState* ConVarRefFloat::clone() const
{
   return new ConVarRefFloat (mAttributes, mValue);
}

   
   
ConVar::EConVarType ConVarRefFloat::getType() const
{
   return ConVar::CVT_REF_FLOAT;
}



string ConVarRefFloat::dumpType() const
{
   return "float ref";
}




void ConVarRefFloat::setValue (float value)
{
   *mValue = value;
   mAttributes.CallBack();
}


float ConVarRefFloat::getValue() const
{
   return *mValue;
}
