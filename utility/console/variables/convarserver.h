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
#ifndef _CONVARSERVER_H_
#define _CONVARSERVER_H_

#include "convar.h"
#include "convarattributes.h"

#include <macros.h>
#include <hash_map>


/** $Id: convarserver.h,v 1.1 2002/08/14 09:24:53 fruit Exp $
  * 
  * ConVarServer - Console Variable Server
  * 
  *      This class provides a pool of ConVars.
  *      It assures that every contained ConVar has a unique name.
  *      It is a singleton and should be retrieved with Instance()
  * 
  * HISTORY:
  * 
  *    08.10.01 - AF
  *       - Initial version
  * 
  *    15.10.01 - AF
  *       - Made singleton implementation more robust
  * 
  *    16.10.01 - AF
  *       - dump no longer prints to stdout but returns a string
  *
  *      18.11.01 - AF
  *         - Documentation added
  * 
  *      31.03.02 - AF
  *         - replaced the self-made hash map with the hash map from stlport
  * 
  * TODO:
  * 
  *   TOFIX:
  */

class ConVarServer 
{
public:
   //! the return type of the method List()
   typedef std::list<std::string> TVarNames;


    //! deletes all contained ConVars
    ~ConVarServer();

    //! these factory methods create new ConVars which are managed (and finally deleted) by the ConVarServer.
   //! for every creatable variable type exists the appropriate create method.
   ConVar* CreateUndefined (const ConVarAttributes& attributes);
   ConVar* CreateBool (const ConVarAttributes& attributes, bool value);
   ConVar* CreateInt (const ConVarAttributes& attributes, int value);
   ConVar* CreateFloat (const ConVarAttributes& attributes, float value);
   ConVar* CreateString (const ConVarAttributes& attributes, const std::string& value);
   ConVar* CreateCharString (const ConVarAttributes& attributes, const char* value);
   ConVar* CreateVector (const ConVarAttributes& attributes, const float x, const float y, const float z);
   ConVar* CreateVector (const ConVarAttributes& attributes, const Vector3& value);
   ConVar* CreateCommand (const ConVarAttributes& attributes, const ConCommand* value);
   ConVar* CreateVariable (const ConVarAttributes& attributes, ConVar* value);
   ConVar* CreateRefBool (const ConVarAttributes& attributes, bool* value);
   ConVar* CreateRefInt (const ConVarAttributes& attributes, int* value);
   ConVar* CreateRefFloat (const ConVarAttributes& attributes, float* value);
   ConVar* CreateRefString (const ConVarAttributes& attributes, std::string* value);
   //! The referenced char string must already be defined (or set to NULL) as it will be deleted on its change.
   ConVar* CreateRefCharString (const ConVarAttributes& attributes, char** value);
   ConVar* CreateRefVector (const ConVarAttributes& attributes, Vector3* value);

   //! does a variable with the given name exist in the pool?
    bool Exists (const std::string& name) const;
   //! returns a ConVar with the given name if it exists, NULL otherwise
   //! this ConVar may not be deleted, it is managed by the ConVarServer
    ConVar* get (const std::string& name) const;
   //! deletes a ConVar from the ConVarServer if it exists and is removable (isRemovable) 
   //! returns true on a successful deletion, false otherwise
    bool Delete (const std::string& name);

   //! retrieves the names of all contained ConVars
   TVarNames List() const;
    
    //! returns a string description of all contained ConVars
   std::string dump() const;
    //! returns a long string description: every attribute
   std::string dumpWithSignature() const;
   
    //! returns the singleton instance
    static ConVarServer& Instance();
    
protected:
   //! the ConVars are stored in this container type
   typedef std::map<std::string, ConVar*> TConVarMap;
   //! the stored ConVars
   TConVarMap mConVarStorage;


    //! ConVarServer is a singleton, so the constructor should be private
    ConVarServer() {};
    ConVarServer (const ConVarServer&) {};
    ConVarServer& operator=(const ConVarServer&) {};

   //! stores the variable under the name
   void Store (const std::string& name, ConVar* variable);
};

#endif // _CONVARSERVER_H_

