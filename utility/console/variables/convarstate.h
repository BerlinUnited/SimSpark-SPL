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
#ifndef UTILITY_CONVARSTATE_H
#define UTILITY_CONVARSTATE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector.h>

#include <vector3.h>

#include "convarattributes.h"
#include "convar.h"

/*! \class ConVarState
 $Id: convarstate.h,v 1.3 2002/08/21 08:18:40 fruit Exp $

    ConVarState
    The base class of the console variable types.  It contains the
    attributes of the variable, a subclass has to define its type and
    its content.

    For every type should exist a set and a get method accepting the
    variable type it stores, e.g. void setInt(int), int getInt().

    A subclass should call M_attributes.callBack() on a change of its
    value.

    The types are managed by the wrapper class ConVar. Look there for
    additional advice how to create and integrate a new variable type,
    and for further documentation.

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Marcus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/

class ConVarState
{
public:
    ConVarState();
    ConVarState(const ConVarAttributes& attributes);
    virtual ~ConVarState();

    /*! It would be nice if the subclasses could use a covariant
       overriding, but VC++ does not support it
     */
    virtual ConVarState* clone() const = 0;

    //! which type has this variable?
    virtual ConVar::ConVarType getType() const = 0;
    //! is this variable of the given type?
    virtual bool isOfType(ConVar::ConVarType type) const;

    //! a set and get method for each variable type 
    virtual bool setBool(bool value);
    virtual bool setInt(int value);
    virtual bool setFloat(TFloat value);
    virtual bool setString(const std::string& value);
    virtual bool setCharString(const char* value);
    virtual bool setVector(const Vector3& value);
    virtual bool setCommand(const ConCommand* value);
    virtual bool setVariable(ConVar* value);
    virtual bool setLabel(const std::string& value);

    virtual bool getBool(bool& value) const;
    virtual bool getInt(int& value) const;
    virtual bool getFloat(TFloat& value) const;
    virtual bool getString(std::string& value) const;
    virtual bool getCharString(char** value) const;
    virtual bool getCommand(const ConCommand** value) const;
    virtual bool getVector(Vector3& value) const;
    virtual bool getVariable(ConVar** value) const;
    virtual bool getLabel(std::string& value) const;

    //! returns the attributes of the variable
    virtual ConVarAttributes getAttributes() const;
    //! sets the attributes of the variable
    virtual void setAttributes(const ConVarAttributes& attributes);

    virtual std::string dumpValue() const = 0;
    virtual std::string dumpType() const = 0;
    //! this is used by the ConExport to export the variable settings
    virtual std::string dump() const;
    virtual std::string dumpWithSignature() const;

protected:
     ConVarAttributes M_attributes;
};

#endif                          // UTILITY_CONVARSTATE_H
