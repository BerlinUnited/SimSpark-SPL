/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2008 RoboCup Soccer Server 3D Maintenance Group
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

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/physicsserver/rigidbody.h>

class Accelerometer : public oxygen::Perceptor {
public:
    Accelerometer();
    virtual ~Accelerometer();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

    virtual void OnLink();

protected:
    virtual void PrePhysicsUpdateInternal(float deltaTime);

private:
    boost::shared_ptr<const oxygen::RigidBody> mBody;
    salt::Vector3f mAcc;
    salt::Vector3f mGravity;
    salt::Vector3f mLastVel;
};

DECLARE_CLASS(Accelerometer);

#endif //ACCELEROMETER_H
