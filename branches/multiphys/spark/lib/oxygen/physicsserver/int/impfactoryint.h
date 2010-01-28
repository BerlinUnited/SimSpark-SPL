/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: hingejoint.h 138 2010-01-08 07:35:25Z a-held $

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
#ifndef OXYGEN_IMPFACTORYINT_H
#define OXYGEN_IMPFACTORYINT_H

#include <oxygen/oxygen_defines.h>
#include <boost/shared_ptr.hpp>

namespace oxygen{

class AngularMotorInt;
class BallJointInt;
class BodyInt;
class BoxColliderInt;
class CapsuleColliderInt;
class ColliderInt;
class CompoundColliderInt;
class ConcaveColliderInt;
class ConeColliderInt;
class ConeTwistJointInt;
class ContactJointHandlerInt;
class ConvexColliderInt;
class CylinderColliderInt;
class DynamicBodyInt;
class EmptyColliderInt;
class FixedJointInt;
class Generic6DOFJointInt;
class Hinge2JointInt;
class HingeJointInt;
class JointInt;
class PhysicsObjectInt;
class PhysicsServerInt;
class PlaneColliderInt;
class RayColliderInt;
class RigidBodyInt;
class SliderJointInt;
class SoftBodyInt;
class SpaceInt;
class SphereColliderInt;
class StaticBodyInt;
class TransformColliderInt;
class UniversalJointInt;
class WorldInt;

class OXYGEN_API ImpFactoryInt{

public:
    virtual boost::shared_ptr<AngularMotorInt> CreateAngularMotorImp() = 0;
    virtual boost::shared_ptr<BallJointInt> CreateBallJointImp() = 0;
    virtual boost::shared_ptr<BodyInt> CreateBodyImp() = 0;
    virtual boost::shared_ptr<BoxColliderInt> CreateBoxColliderImp() = 0;
    virtual boost::shared_ptr<CapsuleColliderInt> CreateCapsuleColliderImp() = 0;
    virtual boost::shared_ptr<ColliderInt> CreateColliderImp() = 0;
    virtual boost::shared_ptr<CompoundColliderInt> CreateCompoundColliderImp() = 0;
    virtual boost::shared_ptr<ConcaveColliderInt> CreateConcaveColliderImp() = 0;
    virtual boost::shared_ptr<ConeColliderInt> CreateConeColliderImp() = 0;
    virtual boost::shared_ptr<ConeTwistJointInt> CreateConeTwistJointImp() = 0;
    virtual boost::shared_ptr<ContactJointHandlerInt> CreateContactJointHandlerImp() = 0;
    virtual boost::shared_ptr<ConvexColliderInt> CreateConvexColliderImp() = 0;
    virtual boost::shared_ptr<CylinderColliderInt> CreateCylinderColliderImp() = 0;
    virtual boost::shared_ptr<DynamicBodyInt> CreateDynamicBodyImp() = 0;
    virtual boost::shared_ptr<EmptyColliderInt> CreateEmptyColliderImp() = 0;
    virtual boost::shared_ptr<FixedJointInt> CreateFixedJointImp() = 0;
    virtual boost::shared_ptr<Generic6DOFJointInt> CreateGeneric6DOFJointImp() = 0;
    virtual boost::shared_ptr<Hinge2JointInt> CreateHinge2JointImp() = 0;
    virtual boost::shared_ptr<HingeJointInt> CreateHingeJointImp() = 0;
    virtual boost::shared_ptr<JointInt> CreateJointImp() = 0;
    virtual boost::shared_ptr<PhysicsObjectInt> CreatePhysicsObjectImp() = 0;
    virtual boost::shared_ptr<PhysicsServerInt> CreatePhysicsServerImp() = 0;
    virtual boost::shared_ptr<PlaneColliderInt> CreatePlaneColliderImp() = 0;
    virtual boost::shared_ptr<RayColliderInt> CreateRayColliderImp() = 0;
    virtual boost::shared_ptr<RigidBodyInt> CreateRigidBodyImp() = 0;
    virtual boost::shared_ptr<SliderJointInt> CreateSliderJointImp() = 0;
    virtual boost::shared_ptr<SoftBodyInt> CreateSoftBodyImp() = 0;
    virtual boost::shared_ptr<SpaceInt> CreateSpaceImp() = 0;
    virtual boost::shared_ptr<SphereColliderInt> CreateSphereColliderImp() = 0;
    virtual boost::shared_ptr<StaticBodyInt> CreateStaticBodyImp() = 0;
    virtual boost::shared_ptr<TransformColliderInt> CreateTransformColliderImp() = 0;
    virtual boost::shared_ptr<UniversalJointInt> CreateUniversalJointImp() = 0;
    virtual boost::shared_ptr<WorldInt> CreateWorldImp() = 0;
};

} //namespace oxygen

#endif //OXYGEN_IMPFACTORYINT_H
