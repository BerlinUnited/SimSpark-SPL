/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: collider.h,v 1.7 2004/03/22 10:57:06 rollmark Exp $

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
#ifndef OXYGEN_COLLIDER_H
#define OXYGEN_COLLIDER_H

#include "odeobject.h"
#include <string>

namespace oxygen
{
class Space;
class World;

/** \class Collider encapsulates an ODE geometry object- geom for
    short. Geoms are the fundamental objects in the collision
    system. They represent a single rigid shape as for example a
    sphere or a box. A special kind of geom called 'space' can
    represent a group of other geoms. A placeable geom can be
    associated with rigid body objects. This allows the collision
    engine to get the position and orientation of the geoms from the
    bodies. A body and a geom together represent all the properties of
    the simulated object.
*/
class Collider : public ODEObject
{
    //
    // Functions
    //
public:
    /** enumerates the two different collision instances reported to
        the OnCollision member for each collision reported from ODE.
     */
    enum ECollisionType
     {
         /** the collision pair as reported from ODE */
         CT_DIRECT,

         /** the symmetric pair to the pair reported from ODE */
         CT_SYMMETRIC
     };

public:
    Collider();
    virtual ~Collider();

    /** notifies the Collider that a collision with another geometry
       object has occured. It is called from Space::HandleCollide.
       For the effects of a collision to be considered in the further
       simulation the Collider has to construct an ODE contact joint
       between the bodies associated to the affected geoms. This task
       among others is delegated to a set of CollisionHandler objects
       registered to a Collider.

       \param collidee is the geom ID of the collision partner

       \param holds the contact points between the two affected geoms
       as returned from ODE dCollide function

       \param symmetric indicates tha this collision indicates a
       symmetric case
    */
    virtual void OnCollision (boost::shared_ptr<Collider> collidee,
                              dContact& contact, ECollisionType type);

    /** registers a new collision handler to this collider. If no
        collision handler is registered until the first call to
        OnCollision a ContactJointHandler is automatically registered.

        \param handlerName is the class name of the CollisionHandler
     */
    bool AddCollisionHandler(const std::string& handlerName);

    /** returns the Collider corresponding to the given ODE geom  */
    static Collider* GetCollider(dGeomID id);

    /** returns the ID of managed ODE geom */
    dGeomID GetODEGeom();

    /** sets the position of the managed geom directly. This is only
        necessary if the geom is not connected to a body. If the geom
        is connected to an ODE body it automatically syncs it's
        position
    */
    void SetPosition(salt::Vector3f pos);

    /** returns true if the ODE geom managed by this
        Collider intersects with the geom managed by the given collider
     */
    bool Intersects(boost::shared_ptr<Collider> collider);

protected:
    /** registers the managed geom to the Space of the Scene and to
        the associated ODE body
    */
    virtual void OnLink();

    /** unregisters the managed geom from the Space of the Scene. The
        geom is removed from the body later within the destructor.
     */
    virtual void OnUnlink();

    /** registers a ContactJointHandler if no handler node was
        registered
    */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    //
    // Members
    //
protected:
    /** the space this collider is associated with */
    boost::shared_ptr<Space> mSpace;

    /** the ode collision geometry */
    dGeomID mODEGeom;
};

DECLARE_CLASS(Collider);

} //namespace oxygen

#endif //OXYGEN_COLLIDER_H
