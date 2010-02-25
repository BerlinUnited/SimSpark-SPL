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
#ifndef OXYGEN_BODY_H
#define OXYGEN_BODY_H

#include <oxygen/physicsserver/physicsobject.h>

namespace oxygen
{
class BodyInt;

/** This is currently a placeholder class to comply with the 
    inheritance tree. Once a Body class other than RigidBody has
    been implemented, common functionality should be moved to this
    common base class to reduce the amount of redundant code.
*/ 

class OXYGEN_API Body : public PhysicsObject
{
public:
    Body();
    virtual ~Body();
    virtual void OnLink();

protected:
    long mBodyID;
    bool mMassTransformed;
    salt::Vector3f mMassTrans;

private:
    static boost::shared_ptr<BodyInt> mBodyImp;
};

DECLARE_CLASS(Body);

} //namespace oxygen

#endif //OXYGEN_BODY_H
