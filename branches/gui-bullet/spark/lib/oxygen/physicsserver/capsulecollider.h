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
#ifndef OXYGEN_CAPSULECOLLIDER_H
#define OXYGEN_CAPSULECOLLIDER_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/convexcollider.h>

namespace oxygen
{
class CapsuleColliderInt;

/** CapsuleCollider encapsulates a capsule geometry
    object. A capsule is like a cylinder except it has
    half-sphere caps at its ends. This feature makes the internal
    collision detection code particularly fast and accurate.
 */
class OXYGEN_API CapsuleCollider : public ConvexCollider
{
    //
    // Functions
    //
public:
    CapsuleCollider();

    /** sets the parameters of the capsule.

       \param radius is the radius of the caps, and of the cylinder itself
       \param length is the height of the cylinder, not counting the caps
    */
    void SetParams(float radius, float length);

    /** sets the radius of the capsule */
    void SetRadius(float radius);

    /** sets the length of the capsule */
    void SetLength(float length);

    /** gets the radius and the length of the capsule */
    void GetParams(float& radius, float& length);

    /** returns the radius of the capsule */
    float GetRadius();

    /** return the length of the capsule */
    float GetLength();

    /** returns the depth of the given relative position in the
       managed capsule geom. Points inside the geom will have
       positive depth, points outside it will have negative depth, and
       points on the surface will have zero depth.
     */
    float GetPointDepth(const salt::Vector3f& pos);

protected:
    /** constructs a default capsule with an radius of 1 and a
        length of 1 
    */
    virtual bool ConstructInternal();
    
private:
    static boost::shared_ptr<CapsuleColliderInt> mCapsuleColliderImp;
};

DECLARE_CLASS(CapsuleCollider);

} //namespace oxygen

#endif //OXYGEN_CAPSULECOLLIDER_H
