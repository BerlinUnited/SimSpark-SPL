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
    for (ConVarMap::iterator iter = M_conVarStorage.begin();
         iter != M_conVarStorage.end(); ++iter)
    {
        delete (*iter).second;
    }
}

ConVarServer& 
ConVarServer::instance()
{
    static ConVarServer sInstance;

    return sInstance;
}

void
ConVarServer::store(const std::string& name, ConVar* variable)
{
    M_conVarStorage[name] = variable;
}

ConVar*
ConVarServer::createUndefined(const ConVarAttributes& attributes)
{
    // if a variable with this name does already exist, exit
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createUndefined(attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createBool(const ConVarAttributes& attributes, bool value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createBool(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createInt(const ConVarAttributes& attributes, int value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createInt(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createFloat(const ConVarAttributes& attributes, TFloat value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createFloat(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createString(const ConVarAttributes& attributes,
                           const string& value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createString(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createCharString(const ConVarAttributes& attributes,
                               const char* value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createCharString(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createVector(const ConVarAttributes& attributes, 
                           TFloat x, TFloat y, TFloat z)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createVector(x, y, z, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createVector(const ConVarAttributes& attributes,
                           const Vector3& value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createVector(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createCommand(const ConVarAttributes& attributes,
                            const ConCommand* value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createCommand(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createVariable(const ConVarAttributes& attributes,
                             ConVar* value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createVariable(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createRefBool(const ConVarAttributes& attributes, bool* value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createRefBool(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createRefInt(const ConVarAttributes& attributes, int* value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createRefInt(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createRefFloat(const ConVarAttributes& attributes, TFloat* value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createRefFloat(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createRefString(const ConVarAttributes& attributes,
                              string* value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createRefString(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createRefCharString(const ConVarAttributes& attributes,
                                  char** value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createRefCharString(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

ConVar*
ConVarServer::createRefVector(const ConVarAttributes& attributes,
                              Vector3* value)
{
    if (exists(attributes.getName()))
    {
        return 0;
    }

    ConVar* con_var = ConVar::createRefVector(value, attributes);
    store(attributes.getName(), con_var);

    return con_var;
}

bool
ConVarServer::exists(const string& name) const
{
    return (M_conVarStorage.find(name) != M_conVarStorage.end());
}

ConVar*
ConVarServer::get(const string& name) const
{
    ConVarMap::const_iterator iter = M_conVarStorage.find(name);

    if (iter == M_conVarStorage.end())
    {
        return 0;
    }

    return (*iter).second;
}

bool
ConVarServer::remove(const string& name)
{
    ConVarMap::iterator iter = M_conVarStorage.find(name);

    if (iter == M_conVarStorage.end())
    {
        return false;
    }

    delete (*iter).second;
    M_conVarStorage.erase(iter);

    return true;
}

ConVarServer::VarNames 
ConVarServer::listNames() const
{
    VarNames variables;

    for (ConVarMap::const_iterator iter = M_conVarStorage.begin();
         iter != M_conVarStorage.end(); ++iter)
    {
        variables.push_back((*iter).first);
    }

    return variables;
}

string
ConVarServer::dump() const
{
    VarNames names = listNames();
    names.sort();

    string dump;
    ConVarMap::const_iterator storage_iter;
    for (VarNames::iterator iter = names.begin(); iter != names.end(); ++iter)
    {
        storage_iter = M_conVarStorage.find(*iter);
        dump += (*storage_iter).second->dump();
    }

    return dump;
}

string
ConVarServer::dumpWithSignature() const
{
    VarNames names = listNames();
    names.sort();

    string dump;
    ConVarMap::const_iterator storage_iter;
    for (VarNames::iterator iter = names.begin(); iter != names.end(); ++iter)
    {
        storage_iter = M_conVarStorage.find(*iter);
        dump += (*storage_iter).second->dumpWithSignature();
    }

    return dump;
}
