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
#ifndef UTILITY_CONVARSERVER_H
#define UTILITY_CONVARSERVER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "convar.h"
#include "convarattributes.h"

//#include <hash_map>
#include <map>

/*! \class ConVarServer
  $Id: convarserver.h,v 1.2 2002/08/21 08:18:39 fruit Exp $

    ConVarServer - Console Variable Server

    This class provides a pool of ConVars.
    It assures that every contained ConVar has a unique name.
    It is a singleton and should be retrieved with instance().

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Markus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/
class ConVarServer
{
public:
    //! the return type of the method listNames()
    typedef std::list < std::string > VarNames;

    //! deletes all contained ConVars
    ~ConVarServer();

    /*! This factory method creates a new ConVar (type undefined)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @return pointer to new variable
     */ 
    ConVar* createUndefined(const ConVarAttributes& attributes);
    /*! This factory method creates a new ConVar (type bool)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createBool(const ConVarAttributes& attributes, bool value);
    /*! This factory method creates a new ConVar (type int)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createInt(const ConVarAttributes& attributes, int value);
    /*! This factory method creates a new ConVar (type float)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createFloat(const ConVarAttributes& attributes, TFloat value);
    /*! This factory method creates a new ConVar (type string)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createString(const ConVarAttributes& attributes,
                         const std::string& value);
    /*! This factory method creates a new ConVar (type char string)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createCharString(const ConVarAttributes& attributes,
                             const char* value);
    /*! This factory method creates a new ConVar (type Vector3)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createVector(const ConVarAttributes& attributes, 
                         TFloat x, TFloat y, TFloat z);
    /*! This factory method creates a new ConVar (type Vector3)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createVector(const ConVarAttributes& attributes,
                         const Vector3& value);
    /*! This factory method creates a new ConVar (type ConCommand)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createCommand(const ConVarAttributes& attributes,
                          const ConCommand* value);
    /*! This factory method creates a new ConVar (type reference to Variable)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createVariable(const ConVarAttributes& attributes, ConVar* value);
    /*! This factory method creates a new ConVar (type reference to bool)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createRefBool(const ConVarAttributes& attributes, bool* value);
    /*! This factory method creates a new ConVar (type reference to int)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createRefInt(const ConVarAttributes& attributes, int* value);
    /*! This factory method creates a new ConVar (type reference to float)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createRefFloat(const ConVarAttributes& attributes, TFloat* value);
    /*! This factory method creates a new ConVar (type reference to string)
        which is managed (and finally deleted) by the ConVarServer.
        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createRefString(const ConVarAttributes& attributes,
                            std::string* value);
    /*! This factory method creates a new ConVar (type reference to
        char string) which is managed (and finally deleted) by the
        ConVarServer.

        The referenced char string must already be defined (or set to
        0) as it will be deleted on its change.

        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
    */ 
    ConVar* createRefCharString(const ConVarAttributes& attributes,
                                char** value);
    /*! This factory method creates a new ConVar (type reference to
        Vector3) which is managed (and finally deleted) by the
        ConVarServer.

        @param attributes variable attributes
        @param value initial value
        @return pointer to new variable
     */ 
    ConVar* createRefVector(const ConVarAttributes& attributes, Vector3* value);

    //! does a variable with the given name exist in the pool?
    bool exists(const std::string& name) const;
    /*! get a variable
        @param name name of variable
        @return a pointer to a ConVar with the given name if it
                exists, 0 otherwise

        This ConVar may not be deleted, it is managed by the ConVarServer.
    */
    ConVar* get(const std::string& name) const;
    /*! delete a ConVar from the ConVarServer if it exists and if it
        is removable (isRemovable)
        @param name name of variable
        @return true on a successful deletion, false otherwise
    */
    bool remove(const std::string& name);

    //! retrieves the names of all contained ConVars
    VarNames listNames() const;

    //! returns a string description of all contained ConVars
    std::string dump() const;
    //! returns a long string description: every attribute
    std::string dumpWithSignature() const;

    //! returns the singleton instance
    static ConVarServer& instance();

protected:
    //! the ConVars are stored in this container type
    typedef std::map < std::string, ConVar* > ConVarMap;
    //! the stored ConVars
    ConVarMap M_conVarStorage;

    //! ConVarServer is a singleton, so the constructor is not public
    ConVarServer() {}
    ConVarServer(const ConVarServer &) {}
    ConVarServer& operator =(const ConVarServer &) {}

    //! stores the variable under the name
    void store(const std::string& name, ConVar* variable);
};
#endif                          // UTILITY_CONVARSERVER_H
