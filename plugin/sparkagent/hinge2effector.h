/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: hinge2effector.h,v 1.1 2004/05/07 12:16:35 rollmark Exp $

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
#ifndef HINGE2EFFECTOR_H
#define HINGE2EFFECTOR_H

#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/hinge2joint.h>

class Hinge2Effector : public oxygen::Effector
{
public:
    Hinge2Effector();
    virtual ~Hinge2Effector();

    /** realizes the action described by the ActionObject */
    virtual bool Realize(boost::shared_ptr<oxygen::ActionObject> action);

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "hinge2"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** setup the reference to the ball body node */
    virtual void OnLink();

    /** remove the reference to the ball body node */
    virtual void OnUnlink();

protected:
    /** cached reference to the monitor joint */
    boost::shared_ptr<oxygen::Hinge2Joint> mJoint;

};

DECLARE_CLASS(Hinge2Effector);

#endif // HINGE2EFFECTOR_H
