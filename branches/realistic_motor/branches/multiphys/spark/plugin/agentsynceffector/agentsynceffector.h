/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentsynceffector.h 131 2009-12-29 19:52:44Z hedayat $

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
#ifndef AGENTSYNCEFFECTOR_H
#define AGENTSYNCEFFECTOR_H

#include <oxygen/agentaspect/effector.h>

class AgentSyncEffector : public oxygen::Effector
{
public:
    AgentSyncEffector();
    virtual ~AgentSyncEffector();

    /** realize the sync action asap */
    virtual bool Realize(boost::shared_ptr<oxygen::ActionObject> action);

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "syn"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** setup the reference to the agent aspect node */
    virtual void OnLink();

    /** remove the reference to the agent aspect node */
    virtual void OnUnlink();

private:
    boost::shared_ptr<oxygen::AgentAspect> mAgentAspect;
};

DECLARE_CLASS(AgentSyncEffector);

#endif // AGENTSYNCEFFECTOR_H
