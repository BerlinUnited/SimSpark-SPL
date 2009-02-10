/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#ifndef FORCEEFFECTOR_H
#define FORCEEFFECTOR_H

#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/body.h>

class ForceEffector : public oxygen::Effector
{
    //
    // functions
    //
public:
    ForceEffector();
    virtual ~ForceEffector();

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "force"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** setup the reference to the agents body node */
    virtual void OnLink();

    /** remove the reference to the agents body node */
    virtual void OnUnlink();

protected:
    /** realizes the action described by the ActionObject */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

protected:
    /** the reference to the parents body node */
    boost::shared_ptr<oxygen::Body> mBody;
};

DECLARE_CLASS(ForceEffector);

#endif // FORCEEFFECTOR_H
