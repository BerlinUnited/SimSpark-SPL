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

#include <oxygen/physicsserver/ode/odeangularmotor.h>
#include <oxygen/physicsserver/ode/odeballjoint.h>
#include <oxygen/physicsserver/ode/odebody.h>
#include <oxygen/physicsserver/ode/odeboxcollider.h>
#include <oxygen/physicsserver/ode/odecapsulecollider.h>
#include <oxygen/physicsserver/ode/odecollider.h>
#include <oxygen/physicsserver/ode/odecompoundcollider.h>
#include <oxygen/physicsserver/ode/odeconcavecollider.h>
#include <oxygen/physicsserver/ode/odeconecollider.h>
#include <oxygen/physicsserver/ode/odeconetwistjoint.h>
#include <oxygen/physicsserver/ode/odecontactjointhandler.h>
#include <oxygen/physicsserver/ode/odeconvexcollider.h>
#include <oxygen/physicsserver/ode/odecylindercollider.h>
#include <oxygen/physicsserver/ode/odedynamicbody.h>
#include <oxygen/physicsserver/ode/odeemptycollider.h>
#include <oxygen/physicsserver/ode/odefixedjoint.h>
#include <oxygen/physicsserver/ode/odegeneric6dofjoint.h>
#include <oxygen/physicsserver/ode/odehinge2joint.h>
#include <oxygen/physicsserver/ode/odehingejoint.h>
#include <oxygen/physicsserver/ode/odejoint.h>
#include <oxygen/physicsserver/ode/odephysicsobject.h>
#include <oxygen/physicsserver/ode/odephysicsserver.h>
#include <oxygen/physicsserver/ode/odeplanecollider.h>
#include <oxygen/physicsserver/ode/oderaycollider.h>
#include <oxygen/physicsserver/ode/oderigidbody.h>
#include <oxygen/physicsserver/ode/odesliderjoint.h>
#include <oxygen/physicsserver/ode/odesoftbody.h>
#include <oxygen/physicsserver/ode/odespace.h>
#include <oxygen/physicsserver/ode/odespherecollider.h>
#include <oxygen/physicsserver/ode/odestaticbody.h>
#include <oxygen/physicsserver/ode/odetransformcollider.h>
#include <oxygen/physicsserver/ode/odeuniversaljoint.h>
#include <oxygen/physicsserver/ode/odeworld.h>

using namespace oxygen;
using namespace boost;

shared_ptr<AngularMotorInt> ODEImpFactory::CreateAngularMotorImp(){
    return shared_ptr<AngularMotorInt>(new ODEAngularMotor());
}

shared_ptr<BallJointInt> ODEImpFactory::CreateBallJointImp(){
    return shared_ptr<BallJointInt>(new ODEBallJoint());
}

shared_ptr<BodyInt> ODEImpFactory::CreateBodyImp(){
    return shared_ptr<BodyInt>(new ODEBody());
}

shared_ptr<BoxColliderInt> ODEImpFactory::CreateBoxColliderImp(){
    return shared_ptr<BoxColliderInt>(new ODEBoxCollider());
}

shared_ptr<CapsuleColliderInt> ODEImpFactory::CreateCapsuleColliderImp(){
    return shared_ptr<CapsuleColliderInt>(new ODECapsuleCollider());
}

shared_ptr<ColliderInt> ODEImpFactory::CreateColliderImp(){
    return shared_ptr<ColliderInt>(new ODECollider());
}

shared_ptr<CompoundColliderInt> ODEImpFactory::CreateCompoundColliderImp(){
    return shared_ptr<CompoundColliderInt>(new ODECompoundCollider());
}

shared_ptr<ConcaveColliderInt> ODEImpFactory::CreateConcaveColliderImp(){
    return shared_ptr<ConcaveColliderInt>(new ODEConcaveCollider());
}

shared_ptr<ConeColliderInt> ODEImpFactory::CreateConeColliderImp(){
    return shared_ptr<ConeColliderInt>(new ODEConeCollider());
}

shared_ptr<ConeTwistJointInt> ODEImpFactory::CreateConeTwistJointImp(){
    return shared_ptr<ConeTwistJointInt>(new ODEConeTwistJoint());
}

shared_ptr<ContactJointHandlerInt> ODEImpFactory::CreateContactJointHandlerImp(){
    return shared_ptr<ContactJointHandlerInt>(new ODEContactJointHandler());
}

shared_ptr<ConvexColliderInt> ODEImpFactory::CreateConvexColliderImp(){
    return shared_ptr<ConvexColliderInt>(new ODEConvexCollider());
}

shared_ptr<CylinderColliderInt> ODEImpFactory::CreateCylinderColliderImp(){
    return shared_ptr<CylinderColliderInt>(new ODECylinderCollider());
}

shared_ptr<DynamicBodyInt> ODEImpFactory::CreateDynamicBodyImp(){
    return shared_ptr<DynamicBodyInt>(new ODEDynamicBody());
}

shared_ptr<EmptyColliderInt> ODEImpFactory::CreateEmptyColliderImp(){
    return shared_ptr<EmptyColliderInt>(new ODEEmptyCollider());
}

shared_ptr<FixedJointInt> ODEImpFactory::CreateFixedJointImp(){
    return shared_ptr<FixedJointInt>(new ODEFixedJoint());
}

shared_ptr<Generic6DOFJointInt> ODEImpFactory::CreateGeneric6DOFJointImp(){
    return shared_ptr<Generic6DOFJointInt>(new ODEGeneric6DOFJoint());
}

shared_ptr<Hinge2JointInt> ODEImpFactory::CreateHinge2JointImp(){
    return shared_ptr<Hinge2JointInt>(new ODEHinge2Joint());
}

shared_ptr<HingeJointInt> ODEImpFactory::CreateHingeJointImp(){
    return shared_ptr<HingeJointInt>(new ODEHingeJoint());
}

shared_ptr<JointInt> ODEImpFactory::CreateJointImp(){
    return shared_ptr<JointInt>(new ODEJoint());
}

shared_ptr<PhysicsObjectInt> ODEImpFactory::CreatePhysicsObjectImp(){
    return shared_ptr<PhysicsObjectInt>(new ODEPhysicsObject());
}

shared_ptr<PhysicsServerInt> ODEImpFactory::CreatePhysicsServerImp(){
    return shared_ptr<PhysicsServerInt>(new ODEPhysicsServer());
}

shared_ptr<PlaneColliderInt> ODEImpFactory::CreatePlaneColliderImp(){
    return shared_ptr<PlaneColliderInt>(new ODEPlaneCollider());
}

shared_ptr<RayColliderInt> ODEImpFactory::CreateRayColliderImp(){
    return shared_ptr<RayColliderInt>(new ODERayCollider());
}

shared_ptr<RigidBodyInt> ODEImpFactory::CreateRigidBodyImp(){
    return shared_ptr<RigidBodyInt>(new ODERigidBody());
}

shared_ptr<SliderJointInt> ODEImpFactory::CreateSliderJointImp(){
    return shared_ptr<SliderJointInt>(new ODESliderJoint());
}

shared_ptr<SoftBodyInt> ODEImpFactory::CreateSoftBodyImp(){
    return shared_ptr<SoftBodyInt>(new ODESoftBody());
}

shared_ptr<SpaceInt> ODEImpFactory::CreateSpaceImp(){
    return shared_ptr<SpaceInt>(new ODESpace());
}

shared_ptr<SphereColliderInt> ODEImpFactory::CreateSphereColliderImp(){
    return shared_ptr<SphereColliderInt>(new ODESphereCollider());
}

shared_ptr<StaticBodyInt> ODEImpFactory::CreateStaticBodyImp(){
    return shared_ptr<StaticBodyInt>(new ODEStaticBody());
}

shared_ptr<TransformColliderInt> ODEImpFactory::CreateTransformColliderImp(){
    return shared_ptr<TransformColliderInt>(new ODETransformCollider());
}

shared_ptr<UniversalJointInt> ODEImpFactory::CreateUniversalJointImp(){
    return shared_ptr<UniversalJointInt>(new ODEUniversalJoint());
}

shared_ptr<WorldInt> ODEImpFactory::CreateWorldImp(){
    return shared_ptr<WorldInt>(new ODEWorld());
}
