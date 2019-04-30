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
#ifndef OXYGEN_PHYSICSSERVER_H
#define OXYGEN_PHYSICSSERVER_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <oxygen/oxygen_defines.h>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <string>

namespace oxygen
{
class PhysicsServerInt;
class Scene;
class RigidBody;
class World;
class Space;

class OXYGEN_API PhysicsServer : public zeitgeist::Leaf
{
    //
    // Functions
    //
public:
    PhysicsServer();
    virtual ~PhysicsServer(){};
    
    virtual void OnLink();
    
    /** Resets the active space and active world */
    void ResetCache();
    
    /** Updates the active world and active space */
    void UpdateCache(boost::shared_ptr<Scene> acticeScene);
    
    /** Collides all objects in the active space */
    void DoCollisions();
    
    /** Advance the simulation by \param deltatime */
    void StepSimulation(float deltaTime); 
    
    /** Creates a new Box and adds it to the SceneGraph */
    void SetUpBox(boost::shared_ptr<RigidBody> body, std::string name);
    
private:
    static boost::shared_ptr<PhysicsServerInt> mPhysicsServerImp;
    
    /** cached reference to the Space node below the active scene */
    boost::shared_ptr<Space> mActiveSpace;

    /** cached reference to the World node below the active scene */
    boost::shared_ptr<World> mActiveWorld;
};

DECLARE_CLASS(PhysicsServer);

} //namespace oxygen

#endif //OXYGEN_PHYSICSSERVER_H
