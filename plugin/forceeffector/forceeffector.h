/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: forceeffector.h,v 1.2.2.1 2003/12/25 13:17:58 rollmark Exp $

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

class ForceEffector : public oxygen::Effector
{
    //
    // functions
    //
public:
    ForceEffector();
    virtual ~ForceEffector();

    /** realizes the action described by the ActionObject */
    virtual bool Realize(boost::shared_ptr<oxygen::ActionObject> action);

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "force"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    //
    // members
    //
private:
    // the accumulated force this effector applies
    salt::Vector3f mForce;
    //! maximum amount of force, this effector can apply
    float mMaxForce;
};

DECLARE_CLASS(ForceEffector);

#endif // FORCEEFFECTOR_H
