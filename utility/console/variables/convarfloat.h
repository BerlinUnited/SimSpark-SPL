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
#ifndef UTILITY_CONVARFLOAT_H
#define UTILITY_CONVARFLOAT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector3.h>

#include "convarstate.h"

/*! \class ConVarFloat
  $Id: convarfloat.h,v 1.2 2002/08/21 08:18:39 fruit Exp $

    ConVarFloat
    This variable contains a floating point number.

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Marcus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/
class ConVarFloat : public ConVarState
{
public:
    ConVarFloat(const ConVarAttributes& attributes);
    ConVarFloat(const ConVarAttributes& attributes, TFloat value);

    virtual ConVarState* clone() const;

    virtual ConVar::ConVarType getType() const;

    virtual bool setInt(int value);
    virtual bool setFloat(TFloat value);
    virtual bool setVariable(ConVar* value);

    virtual bool getFloat(TFloat& value) const;
    virtual bool getString(std::string& value) const;
    virtual bool getCharString(char** value) const;

    virtual std::string dumpValue() const;
    virtual std::string dumpType() const;

protected:
    virtual void setValue(TFloat value);
    virtual TFloat getValue() const;

private:
    TFloat M_value;
};

#endif                          // UTILITY_CONVARFLOAT_H
