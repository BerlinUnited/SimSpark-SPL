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
#ifndef UTILITY_CONVARREFCHARSTRING_H
#define UTILITY_CONVARREFCHARSTRING_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "convarcharstring.h"

/*! \class ConVarRefCharString 
  $Id: convarrefcharstring.h,v 1.2 2002/08/21 08:18:39 fruit Exp $

    ConVarRefCharString

    This variable contains a reference to an old fashioned char*
    string.  The referenced char string must already be defined as it
    will be deleted on its change.

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Markus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/
class ConVarRefCharString : public ConVarCharString
{
public:
    ConVarRefCharString(const ConVarAttributes& attributes, char** value);

    virtual ConVarState* clone() const;

    virtual ConVar::ConVarType getType() const;

    virtual std::string dumpType() const;

protected:

    virtual void setValue(const char* value);
    virtual char* getValue() const;

private:
    char** M_value;
};

#endif                          // UTILITY_CONVARREFCHARSTRING_H
