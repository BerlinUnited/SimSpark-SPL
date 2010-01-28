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
#ifndef OXYGEN_ODEIMPFACTORY_H
#define OXYGEN_ODEIMPFACTORY_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/int/impfactoryint.h>

namespace oxygen{

class OXYGEN_API ODEImpFactory : public ImpFactoryInt{

public:
    boost::shared_ptr<AngularMotorInt> CreateAngularMotorImp();
    boost::shared_ptr<BallJointInt> CreateBallJointImp();
    boost::shared_ptr<BodyInt> CreateBodyImp();
    boost::shared_ptr<BoxColliderInt> CreateBoxColliderImp();
    boost::shared_ptr<CapsuleColliderInt> CreateCapsuleColliderImp();
    boost::shared_ptr<ColliderInt> CreateColliderImp();
    boost::shared_ptr<CompoundColliderInt> CreateCompoundColliderImp();
    boost::shared_ptr<ConcaveColliderInt> CreateConcaveColliderImp();
    boost::shared_ptr<ConeColliderInt> CreateConeColliderImp();
    boost::shared_ptr<ConeTwistJointInt> CreateConeTwistJointImp();
    boost::shared_ptr<ContactJointHandlerInt> CreateContactJointHandlerImp();
    boost::shared_ptr<ConvexColliderInt> CreateConvexColliderImp();
    boost::shared_ptr<CylinderColliderInt> CreateCylinderColliderImp();
    boost::shared_ptr<DynamicBodyInt> CreateDynamicBodyImp();
    boost::shared_ptr<EmptyColliderInt> CreateEmptyColliderImp();
    boost::shared_ptr<FixedJointInt> CreateFixedJointImp();
    boost::shared_ptr<Generic6DOFJointInt> CreateGeneric6DOFJointImp();
    boost::shared_ptr<Hinge2JointInt> CreateHinge2JointImp();
    boost::shared_ptr<HingeJointInt> CreateHingeJointImp();
    boost::shared_ptr<JointInt> CreateJointImp();
    boost::shared_ptr<PhysicsObjectInt> CreatePhysicsObjectImp();
    boost::shared_ptr<PhysicsServerInt> CreatePhysicsServerImp();
    boost::shared_ptr<PlaneColliderInt> CreatePlaneColliderImp();
    boost::shared_ptr<RayColliderInt> CreateRayColliderImp();
    boost::shared_ptr<RigidBodyInt> CreateRigidBodyImp();
    boost::shared_ptr<SliderJointInt> CreateSliderJointImp();
    boost::shared_ptr<SoftBodyInt> CreateSoftBodyImp();
    boost::shared_ptr<SpaceInt> CreateSpaceImp();
    boost::shared_ptr<SphereColliderInt> CreateSphereColliderImp();
    boost::shared_ptr<StaticBodyInt> CreateStaticBodyImp();
    boost::shared_ptr<TransformColliderInt> CreateTransformColliderImp();
    boost::shared_ptr<UniversalJointInt> CreateUniversalJointImp();
    boost::shared_ptr<WorldInt> CreateWorldImp();
};

} //namespace oxygen

#endif //OXYGEN_ODEIMPFACTORY_H
