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
#ifndef UTILITY_CONVARUNDEFINED_H
#define UTILITY_CONVARUNDEFINED_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "convarstate.h"

/*! \class ConVarUndefined 
  $Id: convarundefined.h,v 1.2 2002/08/21 08:18:41 fruit Exp $

    ConVarUndefined 

    This variable represents an undefined and typeless variable.  It
    is used as a placeholder for a variable till its type and value is
    known.  After a set method has been called it replaces itself with
    the proper ConVarState subclass in its wrapper ConVar.

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Marcus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/
class ConVarUndefined : public ConVarState
{
public:
    ConVarUndefined(const ConVarAttributes& attributes);

    virtual ConVarState* clone() const;
    virtual ConVar::ConVarType getType() const;
    virtual std::string dumpValue() const;
    virtual std::string dumpType() const;
};

#endif                          // UTILITY_CONVARUNDEFINED_H
