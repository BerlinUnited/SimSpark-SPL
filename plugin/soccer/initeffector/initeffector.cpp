/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: initeffector.cpp,v 1.1.2.1 2003/12/24 17:48:35 fruit Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "initaction.h"
#include "initeffector.h"
#include <zeitgeist/logserver/logserver.h>

#include <sstream>

using namespace boost;
using namespace oxygen;

InitEffector::InitEffector() : oxygen::Effector()
{
}

InitEffector::~InitEffector()
{
}

bool
InitEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    shared_ptr<InitAction> initAction =
        shared_dynamic_cast<InitAction>(action);

    if (initAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    // we want to change the name of the parent node
    shared_ptr<BaseNode> parent =
        shared_dynamic_cast<BaseNode>(make_shared(GetParent()));

    if (parent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) parent node is not derived from BaseNode\n";
        return false;
    }

    // temporary way of setting the unum.
    std::ostringstream ost;
    ost << "#" << initAction->GetNumber();

    parent->SetName(initAction->GetName() + ost.str());
    return true;
}

shared_ptr<ActionObject>
InitEffector::GetActionObject(const BaseParser::TPredicate& predicate)
{
    if (predicate.name != GetPredicate())
    {
        GetLog()->Error() << "ERROR: (InitEffector) invalid predicate"
                          << predicate.name << "\n";
        return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
    }

    if (predicate.parameter.size() < 1 || predicate.parameter.size() > 2)
    {
        GetLog()->Error() << "ERROR: (InitEffector) predicate has "
                          << predicate.parameter.size() << " parameters.\n";
        return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
    }

    BaseParser::TParameterList::const_iterator i = predicate.parameter.begin();

    std::string name = any_cast<std::string>(*i);
    std::string number = "0";
    ++i;
    if (i != predicate.parameter.end())
        number = any_cast<std::string>(*i);

    int n = std::atoi(number.c_str());
    return shared_ptr<ActionObject>(new InitAction(name,n));
}

