/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: createeffector.h,v 1.2.2.1 2004/01/15 20:59:44 fruit Exp $

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
#ifndef OXYGEN_CREATEEFFECTOR_H
#define OXYGEN_CREATEEFFECTOR_H

#include <oxygen/agentaspect/effector.h>

class CreateEffector : public oxygen::Effector
{
 public:
    CreateEffector();
    virtual ~CreateEffector() {};

    /** Create some script server variables on being linked */
    void OnLink();

    /** realizes the action described by the ActionObject */
    virtual bool Realize(boost::shared_ptr<oxygen::ActionObject> action);

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "create"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

private:
    /** Get the agent mass from the scriptserver.
     *  If the script server does not hold an Agent.Mass variable,
     *  a default value is returned.
     * \return the agent mass as stored in the ScriptServer
     */
    float GetAgentMass() const;
    /** Get the agent radius from the scriptserver.
     *  If the script server does not hold an Agent.Radius variable,
     *  a default value is returned.
     * \return the agent radius as stored in the ScriptServer
     */
    float GetAgentRadius() const;
    /** Get agent max speed from the scriptserver.
     *  The agent maximum speed is used in bodies during the
     *  PostPhysicsUpdateInternal to limit the speed of a body.
     *  If the script server does not hold an Agent.MaxSpeed variable,
     *  a default value is returned.
     * \return the agent maximum speed as stored in the ScriptServer
     */
    float GetAgentMaxSpeed() const;
};

DECLARE_CLASS(CreateEffector);

#endif // OXYGEN_CREATEEFFECTOR_H

