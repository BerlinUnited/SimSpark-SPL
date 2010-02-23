/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: export.cpp 3 2008-11-21 02:38:08Z hedayat $

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

#include <zeitgeist/zeitgeist.h>

#include "odeangularmotor.h"
#include "odeballjoint.h"
#include "odebody.h"
#include "odeboxcollider.h"
#include "odecapsulecollider.h"
#include "odecollider.h"
#include "odecompoundcollider.h"
#include "odeconcavecollider.h"
#include "odeconecollider.h"
#include "odeconetwistjoint.h"
#include "odecontactjointhandler.h"
#include "odeconvexcollider.h"
#include "odecylindercollider.h"
#include "odedynamicbody.h"
#include "odeemptycollider.h"
#include "odefixedjoint.h"
#include "odegeneric6dofjoint.h"
#include "odehinge2joint.h"
#include "odehingejoint.h"
#include "odejoint.h"
#include "odephysicsobject.h"
#include "odephysicsserver.h"
#include "odeplanecollider.h"
#include "oderaycollider.h"
#include "oderigidbody.h"
#include "odesliderjoint.h"
#include "odesoftbody.h"
#include "odespace.h"
#include "odespherecollider.h"
#include "odestaticbody.h"
#include "odetransformcollider.h"
#include "odeuniversaljoint.h"
#include "odeworld.h"

ZEITGEIST_EXPORT_BEGIN()
    ZEITGEIST_EXPORT(AngularMotorImp);
    ZEITGEIST_EXPORT(BallJointImp);
    ZEITGEIST_EXPORT(BodyImp);
    ZEITGEIST_EXPORT(BoxColliderImp);
    ZEITGEIST_EXPORT(CapsuleColliderImp);
    ZEITGEIST_EXPORT(ColliderImp);
    ZEITGEIST_EXPORT(CompoundColliderImp);
    ZEITGEIST_EXPORT(ConcaveColliderImp);
    ZEITGEIST_EXPORT(ConeColliderImp);
    ZEITGEIST_EXPORT(ConeTwistJointImp);
    ZEITGEIST_EXPORT(ContactJointHandlerImp);
    ZEITGEIST_EXPORT(ConvexColliderImp);
    ZEITGEIST_EXPORT(CylinderColliderImp);
    ZEITGEIST_EXPORT(DynamicBodyImp);
    ZEITGEIST_EXPORT(EmptyColliderImp);
    ZEITGEIST_EXPORT(FixedJointImp);
    ZEITGEIST_EXPORT(Generic6DOFJointImp);
    ZEITGEIST_EXPORT(Hinge2JointImp);
    ZEITGEIST_EXPORT(HingeJointImp);
    ZEITGEIST_EXPORT(JointImp);
    ZEITGEIST_EXPORT(PhysicsObjectImp);
    ZEITGEIST_EXPORT(PhysicsServerImp);
    ZEITGEIST_EXPORT(PlaneColliderImp);
    ZEITGEIST_EXPORT(RayColliderImp);
    ZEITGEIST_EXPORT(RigidBodyImp);
    ZEITGEIST_EXPORT(SliderJointImp);
    ZEITGEIST_EXPORT(SoftBodyImp);
    ZEITGEIST_EXPORT(SpaceImp);
    ZEITGEIST_EXPORT(SphereColliderImp);
    ZEITGEIST_EXPORT(StaticBodyImp);
    ZEITGEIST_EXPORT(TransformColliderImp);
    ZEITGEIST_EXPORT(UniversalJointImp);
    ZEITGEIST_EXPORT(WorldImp);
ZEITGEIST_EXPORT_END()
