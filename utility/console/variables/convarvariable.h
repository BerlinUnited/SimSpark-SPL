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
#ifndef UTILITY_CONVARVARIABLE_H
#define UTILITY_CONVARVARIABLE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "convarstate.h"

/*! \class ConVarVariable
  $Id: convarvariable.h,v 1.2 2002/08/21 08:18:41 fruit Exp $

    ConVarVariable

    This variable contains just a reference to another console
    variable, i.e. an object of type ConVar*.

    All methods calls are delegated to the contained ConVar.  The only
    exception is getVariable which returns the contained ConVar.

    As even setVariable is just delegated the contained ConVar can not
    be replaced.  Its value may be changed with the help of
    getVariable.

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Markus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/
class ConVarVariable : public ConVarState
{
public:
    ConVarVariable(const ConVarAttributes& attributes, ConVar* value);

    virtual ConVarState* clone() const;

    virtual ConVar::ConVarType getType() const;

    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool setInt(int value);
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool setFloat(TFloat value);
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool setString(const std::string& value);
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool setCharString(const char* value);
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool setVector(const Vector3& value);
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool setCommand(const ConCommand* value);
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool setVariable(ConVar* value);

    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool getInt(int& value) const;
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool getFloat(TFloat& value) const;
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool getString(std::string& value) const;
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool getCharString(char** value) const;
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool getCommand(const ConCommand** value) const;
    //! the set and get methods are merely delegated to the contained ConVar
    virtual bool getVector(Vector3& value) const;
    //! exception from the getters: the contained ConVar is returned
    virtual bool getVariable(ConVar** value) const;

    /*! This method will never be called as dump and dumpWithSignature
        are overriden they are only defined because they are declared
        as abstract in ConVarState.
    */
    virtual std::string dumpValue() const;
    /*! This methods will never be called as dump and
        dumpWithSignature are overriden they are only defined because
        they are declared as abstract in ConVarState.
    */
    virtual std::string dumpType() const;
    //! the dump methods are merely delegated to the contained ConVar
    virtual std::string dump() const;
    //! the dump methods are merely delegated to the contained ConVar
    virtual std::string dumpWithSignature() const;

private:
     ConVar* M_value;
};

#endif                          // UTILITY_CONVARVARIABLE_H
