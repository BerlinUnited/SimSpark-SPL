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

#include <oxygen/physicsserver/impfactory.h>
#include <oxygen/physicsserver/int/impfactoryint.h>
#include <oxygen/physicsserver/ode/odeimpfactory.h>

#include <oxygen/physicsserver/int/angularmotorint.h>
#include <oxygen/physicsserver/int/balljointint.h>
#include <oxygen/physicsserver/int/bodyint.h>
#include <oxygen/physicsserver/int/boxcolliderint.h>
#include <oxygen/physicsserver/int/capsulecolliderint.h>
#include <oxygen/physicsserver/int/colliderint.h>
#include <oxygen/physicsserver/int/compoundcolliderint.h>
#include <oxygen/physicsserver/int/concavecolliderint.h>
#include <oxygen/physicsserver/int/conecolliderint.h>
#include <oxygen/physicsserver/int/conetwistjointint.h>
#include <oxygen/physicsserver/int/contactjointhandlerint.h>
#include <oxygen/physicsserver/int/convexcolliderint.h>
#include <oxygen/physicsserver/int/cylindercolliderint.h>
#include <oxygen/physicsserver/int/dynamicbodyint.h>
#include <oxygen/physicsserver/int/emptycolliderint.h>
#include <oxygen/physicsserver/int/fixedjointint.h>
#include <oxygen/physicsserver/int/generic6dofjointint.h>
#include <oxygen/physicsserver/int/hinge2jointint.h>
#include <oxygen/physicsserver/int/hingejointint.h>
#include <oxygen/physicsserver/int/jointint.h>
#include <oxygen/physicsserver/int/physicsobjectint.h>
#include <oxygen/physicsserver/int/physicsserverint.h>
#include <oxygen/physicsserver/int/planecolliderint.h>
#include <oxygen/physicsserver/int/raycolliderint.h>
#include <oxygen/physicsserver/int/rigidbodyint.h>
#include <oxygen/physicsserver/int/sliderjointint.h>
#include <oxygen/physicsserver/int/softbodyint.h>
#include <oxygen/physicsserver/int/spaceint.h>
#include <oxygen/physicsserver/int/spherecolliderint.h>
#include <oxygen/physicsserver/int/staticbodyint.h>
#include <oxygen/physicsserver/int/transformcolliderint.h>
#include <oxygen/physicsserver/int/universaljointint.h>
#include <oxygen/physicsserver/int/worldint.h>

using namespace oxygen;
using namespace boost;

boost::shared_ptr<ImpFactory> ImpFactory::mInstance;

ImpFactory::ImpFactory(){
    mImpFactoryImp = boost::shared_ptr<ODEImpFactory>(new ODEImpFactory());
}

ImpFactory::~ImpFactory(){

}

shared_ptr<ImpFactory> ImpFactory::GetInstance(){
    if (mInstance.get() == 0)
        mInstance = shared_ptr<ImpFactory>(new ImpFactory());
    return mInstance;
}

shared_ptr<AngularMotorInt> ImpFactory::GetAngularMotorImp(){
    if (mAngularMotorImp.get() == 0)
        mAngularMotorImp = mImpFactoryImp->CreateAngularMotorImp();
    return mAngularMotorImp;
}

shared_ptr<BallJointInt> ImpFactory::GetBallJointImp(){
    if (mBallJointImp.get() == 0)
        mBallJointImp = mImpFactoryImp->CreateBallJointImp();
    return mBallJointImp;
}

shared_ptr<BodyInt> ImpFactory::GetBodyImp(){
    if (mBodyImp.get() == 0)
        mBodyImp = mImpFactoryImp->CreateBodyImp();
    return mBodyImp;
}

shared_ptr<BoxColliderInt> ImpFactory::GetBoxColliderImp(){
    if (mBoxColliderImp.get() == 0)
        mBoxColliderImp = mImpFactoryImp->CreateBoxColliderImp();
    return mBoxColliderImp;
}

shared_ptr<CapsuleColliderInt> ImpFactory::GetCapsuleColliderImp(){
    if (mCapsuleColliderImp.get() == 0)
        mCapsuleColliderImp = mImpFactoryImp->CreateCapsuleColliderImp();
    return mCapsuleColliderImp;
}

shared_ptr<ColliderInt> ImpFactory::GetColliderImp(){
    if (mColliderImp.get() == 0)
        mColliderImp = mImpFactoryImp->CreateColliderImp();
    return mColliderImp;
}

shared_ptr<CompoundColliderInt> ImpFactory::GetCompoundColliderImp(){
    if (mCompoundColliderImp.get() == 0)
        mCompoundColliderImp = mImpFactoryImp->CreateCompoundColliderImp();
    return mCompoundColliderImp;
}

shared_ptr<ConcaveColliderInt> ImpFactory::GetConcaveColliderImp(){
    if (mConcaveColliderImp.get() == 0)
        mConcaveColliderImp = mImpFactoryImp->CreateConcaveColliderImp();
    return mConcaveColliderImp;
}

shared_ptr<ConeColliderInt> ImpFactory::GetConeColliderImp(){
    if (mConeColliderImp.get() == 0)
        mConeColliderImp = mImpFactoryImp->CreateConeColliderImp();
    return mConeColliderImp;
}

shared_ptr<ConeTwistJointInt> ImpFactory::GetConeTwistJointImp(){
    if (mConeTwistJointImp.get() == 0)
        mConeTwistJointImp = mImpFactoryImp->CreateConeTwistJointImp();
    return mConeTwistJointImp;
}

shared_ptr<ContactJointHandlerInt> ImpFactory::GetContactJointHandlerImp(){
    if (mContactJointHandlerImp.get() == 0)
        mContactJointHandlerImp = mImpFactoryImp->CreateContactJointHandlerImp();
    return mContactJointHandlerImp;
}

shared_ptr<ConvexColliderInt> ImpFactory::GetConvexColliderImp(){
    if (mConvexColliderImp.get() == 0)
        mConvexColliderImp = mImpFactoryImp->CreateConvexColliderImp();
    return mConvexColliderImp;
}

shared_ptr<CylinderColliderInt> ImpFactory::GetCylinderColliderImp(){
    if (mCylinderColliderImp.get() == 0)
        mCylinderColliderImp = mImpFactoryImp->CreateCylinderColliderImp();
    return mCylinderColliderImp;
}

shared_ptr<DynamicBodyInt> ImpFactory::GetDynamicBodyImp(){
    if (mDynamicBodyImp.get() == 0)
        mDynamicBodyImp = mImpFactoryImp->CreateDynamicBodyImp();
    return mDynamicBodyImp;
}

shared_ptr<EmptyColliderInt> ImpFactory::GetEmptyColliderImp(){
    if (mEmptyColliderImp.get() == 0)
        mEmptyColliderImp = mImpFactoryImp->CreateEmptyColliderImp();
    return mEmptyColliderImp;
}

shared_ptr<FixedJointInt> ImpFactory::GetFixedJointImp(){
    if (mFixedJointImp.get() == 0)
        mFixedJointImp = mImpFactoryImp->CreateFixedJointImp();
    return mFixedJointImp;
}

shared_ptr<Generic6DOFJointInt> ImpFactory::GetGeneric6DOFJointImp(){
    if (mGeneric6DOFJointImp.get() == 0)
        mGeneric6DOFJointImp = mImpFactoryImp->CreateGeneric6DOFJointImp();
    return mGeneric6DOFJointImp;
}

shared_ptr<Hinge2JointInt> ImpFactory::GetHinge2JointImp(){
    if (mHinge2JointImp.get() == 0)
        mHinge2JointImp = mImpFactoryImp->CreateHinge2JointImp();
    return mHinge2JointImp;
}

shared_ptr<HingeJointInt> ImpFactory::GetHingeJointImp(){
    if (mHingeJointImp.get() == 0)
        mHingeJointImp = mImpFactoryImp->CreateHingeJointImp();
    return mHingeJointImp;
}

shared_ptr<JointInt> ImpFactory::GetJointImp(){
    if (mJointImp.get() == 0)
        mJointImp = mImpFactoryImp->CreateJointImp();
    return mJointImp;
}

shared_ptr<PhysicsObjectInt> ImpFactory::GetPhysicsObjectImp(){
    if (mPhysicsObjectImp.get() == 0)
        mPhysicsObjectImp = mImpFactoryImp->CreatePhysicsObjectImp();
    return mPhysicsObjectImp;
}

shared_ptr<PhysicsServerInt> ImpFactory::GetPhysicsServerImp(){
    if (mPhysicsServerImp.get() == 0)
        mPhysicsServerImp = mImpFactoryImp->CreatePhysicsServerImp();
    return mPhysicsServerImp;
}

shared_ptr<PlaneColliderInt> ImpFactory::GetPlaneColliderImp(){
    if (mPlaneColliderImp.get() == 0)
        mPlaneColliderImp = mImpFactoryImp->CreatePlaneColliderImp();
    return mPlaneColliderImp;
}

shared_ptr<RayColliderInt> ImpFactory::GetRayColliderImp(){
    if (mRayColliderImp.get() == 0)
        mRayColliderImp = mImpFactoryImp->CreateRayColliderImp();
    return mRayColliderImp;
}

shared_ptr<RigidBodyInt> ImpFactory::GetRigidBodyImp(){
    if (mRigidBodyImp.get() == 0)
        mRigidBodyImp = mImpFactoryImp->CreateRigidBodyImp();
    return mRigidBodyImp;
}

shared_ptr<SliderJointInt> ImpFactory::GetSliderJointImp(){
    if (mSliderJointImp.get() == 0)
        mSliderJointImp = mImpFactoryImp->CreateSliderJointImp();
    return mSliderJointImp;
}

shared_ptr<SoftBodyInt> ImpFactory::GetSoftBodyImp(){
    if (mSoftBodyImp.get() == 0)
        mSoftBodyImp = mImpFactoryImp->CreateSoftBodyImp();
    return mSoftBodyImp;
}

shared_ptr<SpaceInt> ImpFactory::GetSpaceImp(){
    if (mSpaceImp.get() == 0)
        mSpaceImp = mImpFactoryImp->CreateSpaceImp();
    return mSpaceImp;
}

shared_ptr<SphereColliderInt> ImpFactory::GetSphereColliderImp(){
    if (mSphereColliderImp.get() == 0)
        mSphereColliderImp = mImpFactoryImp->CreateSphereColliderImp();
    return mSphereColliderImp;
}

shared_ptr<StaticBodyInt> ImpFactory::GetStaticBodyImp(){
    if (mStaticBodyImp.get() == 0)
        mStaticBodyImp = mImpFactoryImp->CreateStaticBodyImp();
    return mStaticBodyImp;
}

shared_ptr<TransformColliderInt> ImpFactory::GetTransformColliderImp(){
    if (mTransformColliderImp.get() == 0)
        mTransformColliderImp = mImpFactoryImp->CreateTransformColliderImp();
    return mTransformColliderImp;
}

shared_ptr<UniversalJointInt> ImpFactory::GetUniversalJointImp(){
    if (mUniversalJointImp.get() == 0)
        mUniversalJointImp = mImpFactoryImp->CreateUniversalJointImp();
    return mUniversalJointImp;
}

shared_ptr<WorldInt> ImpFactory::GetWorldImp(){
    if (mWorldImp.get() == 0)
        mWorldImp = mImpFactoryImp->CreateWorldImp();
    return mWorldImp;
}
