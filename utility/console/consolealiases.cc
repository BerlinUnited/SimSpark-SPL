/* -*-c++-*- ***************************************************************
                           consolealiases.cc
                           ------------------------
    begin                : Aug 16 2002  Oliver Obst
    copyright            : (C) 2002 by The RoboCup Soccer Simulator
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU GPL as published by the Free Software   *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/

#include "consolealiases.h"

using namespace std;

bool
ConsoleAliases::exists(const string& alias) const
{
    return (M_aliases.find(alias) != M_aliases.end());
}

void
ConsoleAliases::set(const string& alias, const string& value)
{
    M_aliases[alias] = value;
}

string
ConsoleAliases::get(const string& alias) const
{
    Aliases::const_iterator iter = M_aliases.find(alias);

    if (iter != M_aliases.end())
    {
        return (*iter).second;
    }

    return "";
}

ConsoleAliases::StringList 
ConsoleAliases::getAliases() const
{
    StringList aliases;

    for (Aliases::const_iterator iter = M_aliases.begin();
         iter != M_aliases.end(); ++iter)
    {
        aliases.push_back((*iter).first);
    }

    return aliases;
}

void
ConsoleAliases::remove(const string& alias)
{
    M_aliases.erase(alias);
}
