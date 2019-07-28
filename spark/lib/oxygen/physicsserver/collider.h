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
#ifndef OXYGEN_COLLIDER_H
#define OXYGEN_COLLIDER_H

#include <oxygen/physicsserver/physicsobject.h>
#include <oxygen/oxygen_defines.h>
#include <string>
#include <set>

namespace oxygen
{
class ColliderInt;
class GenericContact;

/** \class Collider encapsulates a geometry object- geom for
    short. Geoms are the fundamental objects in the collision
    system. They represent a single rigid shape as for example a
    sphere or a box. A special kind of geom called 'space' can
    represent a group of other geoms. A placeable geom can be
    associated with rigid body objects. This allows the collision
    engine to get the position and orientation of the geoms from the
    bodies. A body and a geom together represent all the properties of
    the simulated object.
*/
class OXYGEN_API Collider : public PhysicsObject
{
    //
    // Functions
    //
public:
    /** enumerates the two different collision instances reported to
        the OnCollision member for each collision reported from the
        physics engine.
     */
    enum ECollisionType
     {
         /** the collision pair as reported from the physics engine */
         CT_DIRECT,

         /** the symmetric pair to the pair reported from the
             physics engine
         */
         CT_SYMMETRIC
     };

    /** TColliderNameSet is a set that store the collider name */
    typedef std::set<std::string> TColliderNameSet;

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

       \param symmetric indicates that this collision indicates a
       symmetric case
    */
    virtual void OnCollision (boost::shared_ptr<Collider> collidee,
                              GenericContact& contact, ECollisionType type);

    /** registers a new collision handler to this collider. If no
        collision handler is registered until the first call to
        OnCollision a ContactJointHandler is automatically registered.

        \param handlerName is the class name of the CollisionHandler
     */
    bool AddCollisionHandler(const std::string& handlerName);

    /** returns the Collider corresponding to the given geom  */
    static boost::shared_ptr<Collider> GetCollider(long geomID);

    /** returns the ID of managed geom */
    long GetGeomID();

    /** sets the relative position of the managed geom directly. If
        the geom is connected to a body, the position of the body will
        also be changed
    */
    virtual void SetPosition(const salt::Vector3f& pos);

    /** sets the relative position of the managed geom to the body's center.
        Only use for colliders encapsulated in a TransformCollider!
    */
    virtual void SetLocalPosition(const salt::Vector3f& pos);

    /** returns the absolute position of the managed geom */
    salt::Vector3f GetPosition() const;

    /** sets the relative orientation of the managed geom directly. If
        the geom is connected to a body, the orientation of the body
        will also be changed
     */
    virtual void SetRotation(const salt::Matrix& rot);

    /** returns true if the geom managed by this
        Collider intersects with the geom managed by the given collider
     */
    bool Intersects(boost::shared_ptr<Collider> collider);

    /** returns the handle ID of the containing parent space */
    virtual long GetParentSpaceID();

    /** Add/Reomve a collider name that not collide with */
    void AddNotCollideWithColliderName(const std::string& colliderName, bool isAdd);

    /** Get the not collide with collider set */
    const TColliderNameSet& GetNotCollideWithSet() const;

    bool InNotCollideWithSet( boost::shared_ptr<Collider>  col2 );

protected:
    /** registers the managed geom to the Space of the Scene and to
        the associated body
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

    /** destroy the managed physicsobject */
    virtual void DestroyPhysicsObject();

    //
    // Members
    //
protected:
    /** the ID of the managed collision geometry */
    long mGeomID;
    
private:
    static boost::shared_ptr<ColliderInt> mColliderImp;

protected:
    /** a set that store the colliders that I never collide with
        Note: they should be in the same space, or else this is ignored
     */
    TColliderNameSet mNotCollideWithSet;
};

DECLARE_CLASS(Collider);

} //namespace oxygen

#endif //OXYGEN_COLLIDER_H
