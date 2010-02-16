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
#include <oxygen/oxygen.h>

using namespace oxygen;
using namespace zeitgeist;

Oxygen::Oxygen(zeitgeist::Zeitgeist& zg)
{
    // scene graph
    zg.GetCore()->RegisterClassObject(new CLASS(SceneServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BaseNode), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Camera), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(FPSController), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Scene), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Transform), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SceneImporter), "oxygen/");

    // simulation
    zg.GetCore()->RegisterClassObject(new CLASS(SimulationServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SimControlNode), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(NetControl), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(NetMessage), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(AgentControl), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorControl), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorLogger), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(TrainControl), "oxygen/");

    // geometry
    zg.GetCore()->RegisterClassObject(new CLASS(GeometryServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(MeshExporter), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(MeshImporter), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(StdMeshImporter), "oxygen/");

    // physics
    zg.GetCore()->RegisterClassObject(new CLASS(PhysicsServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(PhysicsObject), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Body), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(RigidBody), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(StaticBody), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(DynamicBody), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SoftBody), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BodyController), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(DragController), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(VelocityController), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Space), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(World), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Joint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Generic6DOFJoint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(AngularMotor), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BallJoint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ConeTwistJoint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(HingeJoint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(FixedJoint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SliderJoint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(UniversalJoint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Hinge2Joint), "oxygen/");

    // collider
    zg.GetCore()->RegisterClassObject(new CLASS(Collider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(CompoundCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ConcaveCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ConeCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ConvexCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(EmptyCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(TransformCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BoxCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(CapsuleCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(CylinderCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(PlaneCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(RayCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SphereCollider), "oxygen/");

    // collision handler
    zg.GetCore()->RegisterClassObject(new CLASS(CollisionHandler), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ContactJointHandler), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(RecorderHandler), "oxygen/");
    
    // implementations
    zg.GetCore()->RegisterClassObject(new CLASS(AngularMotorInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BallJointInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BodyInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BoxColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(CapsuleColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(CompoundColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ConcaveColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ConeColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ConeTwistJointInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ContactJointHandlerInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ConvexColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(CylinderColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(DynamicBodyInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(EmptyColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(FixedJointInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Generic6DOFJointInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Hinge2JointInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(HingeJointInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(JointInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(PhysicsObjectInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(PhysicsServerInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(PlaneColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(RayColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(RigidBodyInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SliderJointInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SoftBodyInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SpaceInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SphereColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(StaticBodyInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(TransformColliderInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(UniversalJointInt), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(WorldInt), "oxygen/");

    // agentaspect
    zg.GetCore()->RegisterClassObject(new CLASS(AgentAspect), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Effector), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Perceptor), "oxygen/");

#ifdef HAVE_SPADES_HEADERS
    // spades
    zg.GetCore()->RegisterClassObject(new CLASS(SpadesServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ParamReader), "oxygen/");
#endif

    // monitor
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorSystem), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(CustomMonitor), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorItem),    "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorCmdParser),"oxygen/");

    // game control and controlaspects
    zg.GetCore()->RegisterClassObject(new CLASS(BaseParser), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(GameControlServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ControlAspect), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ActionObject), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Behavior), "oxygen/");

    // run the oxygen init script
    zg.GetCore()->GetRoot()->GetScript()->RunInitScript
        (
         "oxygen.rb",
         "lib/oxygen",
         ScriptServer::IS_COMMON
         );
}
