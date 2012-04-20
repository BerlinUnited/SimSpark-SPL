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
#ifndef OXYGEN_PLANECOLLIDER_H
#define OXYGEN_PLANECOLLIDER_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/collider.h>

namespace oxygen
{
class PlaneColliderInt;

/** PlaneCollider encapsulates an ODE plane geometry object. Planes
    are non-placeable geoms, i.e.  unlike placeable geoms, planes do
    not have an assigned position and rotation. This means that the
    parameters (a,b,c,d) are always in global coordinates. In other
    words it is assumed that the plane is always part of the static
    environment and not tied to any movable object.
*/
class OXYGEN_API PlaneCollider : public Collider
{
    //
    // Functions
    //
public:
    PlaneCollider();

    /** sets the parameters of the plane equation a*x+b*y+c*z = d ,
        all parameters are given in global coordinates
     */
    void SetParams(float a, float b, float c, float d);

    /** set the parameters of the plane given a position and a normal
        vector; all parameters are given in global coordinates
    */
    void SetParams(const salt::Vector3f& pos, salt::Vector3f normal);

    /** returns the depth of the given relative position in the
        managed plane. Points inside the geom will have positive
        depth, points outside it will have negative depth, and points
        on the surface will have zero depth.
    */
    float GetPointDepth(const salt::Vector3f& pos);

    /** this is a stub method that does not nothing, as a plane is a non
        placeable geom.
    */
    virtual void SetPosition(const salt::Vector3f& pos);

    /** this is a stub method that does nothing, as a plane is a non
        placeable geom
     */
    virtual void SetRotation(const salt::Matrix& rot);

protected:
    /** constructs a default plane with normal pointing up, going
        through the origin */
    virtual bool ConstructInternal();
    
private:
    static boost::shared_ptr<PlaneColliderInt> mPlaneColliderImp;
};

DECLARE_CLASS(PlaneCollider);

} //namespace oxygen

#endif //OXYGEN_PLANECOLLIDER_H
