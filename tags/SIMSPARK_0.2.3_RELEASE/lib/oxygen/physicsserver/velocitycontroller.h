/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef OXYGEN_VELOCITYCONTROLLER_H
#define OXYGEN_VELOCITYCONTROLLER_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/bodycontroller.h>

namespace oxygen
{

/** \class VelocityController is a BodyController that restricts the
    maximum velocity a body can attain
 */
class OXYGEN_API VelocityController : public BodyController
{
    //
    // Functions
    //
public:
    VelocityController();
    virtual ~VelocityController();

    // returns the maximum velocity
    float GetMaxVelocity();

    // sets the maximum velocity
    void SetMaxVelocity(float vel);

protected:
    /** enforces the maximum allowed velocity */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    //
    // Members
    //
protected:
    /** maximum velocity, the controlled body can attain */
    float mMaxVel;
};

DECLARE_CLASS(VelocityController);

} // namespace oxygen


#endif // OXYGEN_VELOCITYCONTROLLER_H
