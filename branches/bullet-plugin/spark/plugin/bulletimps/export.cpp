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

#include <zeitgeist/zeitgeist.h>

#include "bulletangularmotor.h"
#include "bulletballjoint.h"
#include "bulletbody.h"
#include "bulletboxcollider.h"
#include "bulletcapsulecollider.h"
#include "bulletcollider.h"
#include "bulletcompoundcollider.h"
#include "bulletconcavecollider.h"
#include "bulletconecollider.h"
#include "bulletconetwistjoint.h"
#include "bulletcontactjointhandler.h"
#include "bulletconvexcollider.h"
#include "bulletcylindercollider.h"
#include "bulletdynamicbody.h"
#include "bulletemptycollider.h"
#include "bulletfixedjoint.h"
#include "bulletgeneric6dofjoint.h"
#include "bullethinge2joint.h"
#include "bullethingejoint.h"
#include "bulletjoint.h"
#include "bulletphysicsobject.h"
#include "bulletphysicsserver.h"
#include "bulletplanecollider.h"
#include "bulletraycollider.h"
#include "bulletrigidbody.h"
#include "bulletsliderjoint.h"
#include "bulletsoftbody.h"
#include "bulletspace.h"
#include "bulletspherecollider.h"
#include "bulletstaticbody.h"
#include "bullettransformcollider.h"
#include "bulletuniversaljoint.h"
#include "bulletworld.h"

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
