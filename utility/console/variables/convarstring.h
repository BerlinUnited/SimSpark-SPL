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
#ifndef _CONVARSTRING_H_
#define _CONVARSTRING_H_

#include "convarstate.h"

/** $Id: convarstring.h,v 1.1 2002/08/14 09:24:53 fruit Exp $
  * 
  *   ConVarString
  * 
  *      This variable contains a string.
  *
  *   HISTORY:
  *       04.02.02 - AF
  *         - First working version
  * 
  *   TODO:
  * 
  *   TOFIX:
  */

class ConVarString : public ConVarState
{
public:
   ConVarString (const ConVarAttributes& attributes);
   ConVarString (const ConVarAttributes& attributes, const std::string& value);
   
   virtual ConVarState* clone() const;
   
   virtual ConVar::EConVarType getType() const;
   
   virtual bool setInt (int value);
   virtual bool setFloat (float value);
   virtual bool setString (const std::string& value);
   virtual bool setCharString (const char* value);
   virtual bool setVariable (ConVar* value);
   
   virtual bool getString (std::string& value) const;
   virtual bool getCharString (char** value) const;
   
   virtual std::string dumpValue() const;
   virtual std::string dumpType() const;
   
protected:
   virtual void setValue (std::string value);
   virtual std::string getValue() const;

private:
   std::string mValue;
};


#endif // _CONVARSTRING_H_

