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
#ifndef OXYGEN_BOXCOLLIDER_H
#define OXYGEN_BOXCOLLIDER_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/convexcollider.h>
#include <salt/vector.h>

namespace oxygen
{
class BoxColliderInt;

/** BoxCollider encapsulates an ODE box geometry object.
 */
class OXYGEN_API BoxCollider : public ConvexCollider
{
    //
    // Functions
    //
public:
    BoxCollider();

    /** sets the side lengths of the box geom */
    void SetBoxLengths(const salt::Vector3f& extents);

    /** gets the side lengths of the box geom */
    void GetBoxLengths(salt::Vector3f& extents);

    /** gets the length of on side of the box

        \param axis gives the queried axis, 0-X, 1-Y, 2-Z
     */
    float GetBoxLength(int axis);

    /** returns the depth of the given relative position in the
       managed box geom. Points inside the geom will have positive
       depth, points outside it will have negative depth, and points
       on the surface will have zero depth.
     */
    float GetPointDepth(const salt::Vector3f& pos);

    bool CheckSeparatingPlane(const salt::Vector3f& RPos, const salt::Vector3f& Plane, boost::shared_ptr<BoxCollider> box2, float tol=0.0);
    bool CheckCollisions( boost::shared_ptr<BoxCollider> box2, float tol=0.0);

    void AddSCFreezeJointEffName(const std::string name);
    const std::list<std::string> GetSCFreezeJointEffNames();

protected:
    /** constructs a default box with side lengths of 1 */
    virtual bool ConstructInternal();
    
private:
    static boost::shared_ptr<BoxColliderInt> mBoxColliderImp;
    std::list<std::string> selfCollisionFreezeJointEffNames;
};

DECLARE_CLASS(BoxCollider);

} //namespace oxygen

#endif //OXYGEN_BOXCOLLIDER_H
