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
#ifndef OXYGEN_IMPFACTORY_H
#define OXYGEN_IMPFACTORY_H

#include <oxygen/oxygen_defines.h>
#include <boost/shared_ptr.hpp>

namespace oxygen{

class ImpFactoryInt;
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

class OXYGEN_API ImpFactory{

public:
    ImpFactory();
    virtual ~ImpFactory();
    
    static boost::shared_ptr<ImpFactory> GetInstance();
    boost::shared_ptr<AngularMotorInt> GetAngularMotorImp();
    boost::shared_ptr<BallJointInt> GetBallJointImp();
    boost::shared_ptr<BodyInt> GetBodyImp();
    boost::shared_ptr<BoxColliderInt> GetBoxColliderImp();
    boost::shared_ptr<CapsuleColliderInt> GetCapsuleColliderImp();
    boost::shared_ptr<ColliderInt> GetColliderImp();
    boost::shared_ptr<CompoundColliderInt> GetCompoundColliderImp();
    boost::shared_ptr<ConcaveColliderInt> GetConcaveColliderImp();
    boost::shared_ptr<ConeColliderInt> GetConeColliderImp();
    boost::shared_ptr<ConeTwistJointInt> GetConeTwistJointImp();
    boost::shared_ptr<ContactJointHandlerInt> GetContactJointHandlerImp();
    boost::shared_ptr<ConvexColliderInt> GetConvexColliderImp();
    boost::shared_ptr<CylinderColliderInt> GetCylinderColliderImp();
    boost::shared_ptr<DynamicBodyInt> GetDynamicBodyImp();
    boost::shared_ptr<EmptyColliderInt> GetEmptyColliderImp();
    boost::shared_ptr<FixedJointInt> GetFixedJointImp();
    boost::shared_ptr<Generic6DOFJointInt> GetGeneric6DOFJointImp();
    boost::shared_ptr<Hinge2JointInt> GetHinge2JointImp();
    boost::shared_ptr<HingeJointInt> GetHingeJointImp();
    boost::shared_ptr<JointInt> GetJointImp();
    boost::shared_ptr<PhysicsObjectInt> GetPhysicsObjectImp();
    boost::shared_ptr<PhysicsServerInt> GetPhysicsServerImp();
    boost::shared_ptr<PlaneColliderInt> GetPlaneColliderImp();
    boost::shared_ptr<RayColliderInt> GetRayColliderImp();
    boost::shared_ptr<RigidBodyInt> GetRigidBodyImp();
    boost::shared_ptr<SliderJointInt> GetSliderJointImp();
    boost::shared_ptr<SoftBodyInt> GetSoftBodyImp();
    boost::shared_ptr<SpaceInt> GetSpaceImp();
    boost::shared_ptr<SphereColliderInt> GetSphereColliderImp();
    boost::shared_ptr<StaticBodyInt> GetStaticBodyImp();
    boost::shared_ptr<TransformColliderInt> GetTransformColliderImp();
    boost::shared_ptr<UniversalJointInt> GetUniversalJointImp();
    boost::shared_ptr<WorldInt> GetWorldImp();
    
protected:
    boost::shared_ptr<ImpFactoryInt> mImpFactoryImp;

private:
    static boost::shared_ptr<ImpFactory> mInstance;
    boost::shared_ptr<AngularMotorInt> mAngularMotorImp;
    boost::shared_ptr<BallJointInt> mBallJointImp;
    boost::shared_ptr<BodyInt> mBodyImp;
    boost::shared_ptr<BoxColliderInt> mBoxColliderImp;
    boost::shared_ptr<CapsuleColliderInt> mCapsuleColliderImp;
    boost::shared_ptr<ColliderInt> mColliderImp;
    boost::shared_ptr<CompoundColliderInt> mCompoundColliderImp;
    boost::shared_ptr<ConcaveColliderInt> mConcaveColliderImp;
    boost::shared_ptr<ConeColliderInt> mConeColliderImp;
    boost::shared_ptr<ConeTwistJointInt> mConeTwistJointImp;
    boost::shared_ptr<ContactJointHandlerInt> mContactJointHandlerImp;
    boost::shared_ptr<ConvexColliderInt> mConvexColliderImp;
    boost::shared_ptr<CylinderColliderInt> mCylinderColliderImp;
    boost::shared_ptr<DynamicBodyInt> mDynamicBodyImp;
    boost::shared_ptr<EmptyColliderInt> mEmptyColliderImp;
    boost::shared_ptr<FixedJointInt> mFixedJointImp;
    boost::shared_ptr<Generic6DOFJointInt> mGeneric6DOFJointImp;
    boost::shared_ptr<Hinge2JointInt> mHinge2JointImp;
    boost::shared_ptr<HingeJointInt> mHingeJointImp;
    boost::shared_ptr<JointInt> mJointImp;
    boost::shared_ptr<PhysicsObjectInt> mPhysicsObjectImp;
    boost::shared_ptr<PhysicsServerInt> mPhysicsServerImp;
    boost::shared_ptr<PlaneColliderInt> mPlaneColliderImp;
    boost::shared_ptr<RayColliderInt> mRayColliderImp;
    boost::shared_ptr<RigidBodyInt> mRigidBodyImp;
    boost::shared_ptr<SliderJointInt> mSliderJointImp;
    boost::shared_ptr<SoftBodyInt> mSoftBodyImp;
    boost::shared_ptr<SpaceInt> mSpaceImp;
    boost::shared_ptr<SphereColliderInt> mSphereColliderImp;
    boost::shared_ptr<StaticBodyInt> mStaticBodyImp;
    boost::shared_ptr<TransformColliderInt> mTransformColliderImp;
    boost::shared_ptr<UniversalJointInt> mUniversalJointImp;
    boost::shared_ptr<WorldInt> mWorldImp;    
};

} //namespace oxygen

#endif //OXYGEN_IMPFACTORY_H
