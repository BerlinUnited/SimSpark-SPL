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
#include "convarserver.h"

#include <algorithm>

using namespace std;


ConVarServer::~ConVarServer()
{
   for (TConVarMap::iterator iter = mConVarStorage.begin(); iter != mConVarStorage.end(); ++iter)
   {
      delete (*iter).second;
   }
}

ConVarServer& ConVarServer::Instance()
{
    static ConVarServer sInstance;
    
    return sInstance;
}



void ConVarServer::Store (const std::string& name, ConVar* variable)
{
   mConVarStorage[name] = variable;
}



ConVar* ConVarServer::CreateUndefined (const ConVarAttributes& attributes)
{
   // if a variable with this name does already exist, exit
    if (Exists (attributes.getName())) {
        return NULL;
    }

   ConVar* conVar = ConVar::CreateUndefined (attributes);
   Store (attributes.getName(), conVar);

   return conVar;
}

ConVar* ConVarServer::CreateBool (const ConVarAttributes& attributes, bool value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateBool (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateInt (const ConVarAttributes& attributes, int value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateInt (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateFloat (const ConVarAttributes& attributes, float value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateFloat (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateString (const ConVarAttributes& attributes, const string& value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateString (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateCharString (const ConVarAttributes& attributes, const char* value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateCharString (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateVector (const ConVarAttributes& attributes, const float x, const float y, const float z)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateVector (x, y, z, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateVector (const ConVarAttributes& attributes, const Vector3& value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateVector (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateCommand (const ConVarAttributes& attributes, const ConCommand* value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateCommand (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateVariable (const ConVarAttributes& attributes, ConVar* value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateVariable (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateRefBool (const ConVarAttributes& attributes, bool* value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateRefBool (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateRefInt (const ConVarAttributes& attributes, int* value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateRefInt (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateRefFloat (const ConVarAttributes& attributes, float* value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateRefFloat (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateRefString (const ConVarAttributes& attributes, string* value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateRefString (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateRefCharString (const ConVarAttributes& attributes, char** value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateRefCharString (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}

ConVar* ConVarServer::CreateRefVector (const ConVarAttributes& attributes, Vector3* value)
{
    if (Exists (attributes.getName())) {
        return NULL;
    }
   
   ConVar* conVar = ConVar::CreateRefVector (value, attributes);
   Store (attributes.getName(), conVar);
   
   return conVar;
}






bool ConVarServer::Exists (const string& name) const
{
    return (mConVarStorage.find (name) != mConVarStorage.end());
}

ConVar* ConVarServer::get (const string& name) const
{
   TConVarMap::const_iterator iter = mConVarStorage.find (name);
   
   if (iter == mConVarStorage.end()) 
   {
      return NULL;
   }

   return (*iter).second;
}

bool ConVarServer::Delete (const string& name)
{
   TConVarMap::iterator iter = mConVarStorage.find (name);
   
   if (iter == mConVarStorage.end()) 
   {
      return false;
   }

   delete (*iter).second;
   mConVarStorage.erase (iter);

   return true;
}



ConVarServer::TVarNames ConVarServer::List() const
{
   TVarNames variables;

   for (TConVarMap::const_iterator iter = mConVarStorage.begin(); iter != mConVarStorage.end(); ++iter)
   {
      variables.push_back ((*iter).first);
   }

   return variables;
}




string ConVarServer::dump() const
{
   TVarNames names = List();
   names.sort();

   string dump;
   TConVarMap::const_iterator storageIter;
   for (TVarNames::iterator iter = names.begin(); iter != names.end(); ++iter)
   {
      storageIter = mConVarStorage.find (*iter);
      dump += (*storageIter).second->dump();
   }

   return dump;
}

string ConVarServer::dumpWithSignature() const
{
   TVarNames names = List();
   names.sort();
   
   string dump;
   TConVarMap::const_iterator storageIter;
   for (TVarNames::iterator iter = names.begin(); iter != names.end(); ++iter)
   {
      storageIter = mConVarStorage.find (*iter);
      dump += (*storageIter).second->dumpWithSignature();
   }
   
   return dump;
}


