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
#ifndef OXYGEN_CONTACTJOINTHANDLER_H
#define OXYGEN_CONTACTJOINTHANDLER_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{
class ContactJointHandlerInt;

/** \class ContactJointHandler is a CollisionHandler that creates a
    contact joint between the two bodies associated with the two
    affected collision geoms.
 */
class OXYGEN_API ContactJointHandler : public CollisionHandler
{
public:
    ContactJointHandler();
    virtual ~ContactJointHandler();
    
    virtual void OnLink();

    /** Check if the collidee also has a ContactJoint handler
        registered to it. If yes, create a contact joint between the
        bodies corresponding to our Collider and the collidee's geom
        using the surface parameters stored in the mSurfaceParameter
        member.

        \param collidee is the geom ID of the colliders collision
        partner

        \param holds the contact points between the two affected geoms
        as returned from the ODE dCollide function
    */
    virtual void HandleCollision
    (boost::shared_ptr<Collider> collidee, GenericContact& contact);

    /** the ContactJointHandler is not a symmetric handler. See
        CollisionHandler::IsSymmetricHandler for an explanation
    */
    virtual bool IsSymmetricHandler() { return false; }

    /** sets the surface parameters for the contact joints that the
        CollisionHandler creates
    */
    void SetSurfaceParameter(const GenericSurfaceParameter& surface);

    /** returns the surface parameters for the contact joints that the
        CollisionHandler creates
    */
    GenericSurfaceParameter& GetSurfaceParameter() const;

    /** returns the current set of contact mode flags in the surface
        parameter
     */
    int GetContactMode() const;

    /** sets or resets Bounce mode flag */
    void SetContactBounceMode(bool set);

    /** sets the bounce value */
    void SetBounceValue(float bounce);

    /** returns the bounce value */
    float GetBounceValue() const;

    /** sets the mininum incoming velocity necessary for bounce */
    void SetMinBounceVel(float vel);

    /** returns the mininum incoming velocity necessary for bounce */
    float GetMinBounceVel() const;

    /** sets or resets the error reduction parameter (ERP) mode,
        useful to make surfaces soft
    */
    void SetContactSoftERPMode(bool set);

    /** sets the contact normal error reduction parameter (ERP) */
    void SetContactSoftERP(float erp);

    /** returns the contact normal error reduction parameter (ERP) */
    float GetContactSoftERP() const;

    /** sets or resets the constraint force mixing mode (CFM), useful
        to make surfaces soft
    */
    void SetContactSoftCFMMode(bool set);

    /** sets the constraint force mixing parameter (CFM) */
    void SetContactSoftCFM(float cfm);

    /** returns the constraint force mixing parameter (CFM) */
    float GetContactSoftCFM() const;

    /** sets or resets the force dependent contact slip mode (FDS) */
    void SetContactSlipMode (bool set);

    /** sets the force dependent slip (FDS) in both friction
        directions
    */
    void SetContactSlip(float slip);

    /** returns the force dependent slip in the first slip direction
        (FDS)
    */
    float GetContactSlip1() const;

    /** returns the force dependent slip in the second slip direction
        (FDS)
    */
    float GetContactSlip2() const;

    /** sets the Coulomb friction coefficient */
    void SetContactMu(float mu);

    /** returns the Coulomb friction coefficient */
    float GetContactMu() const;
    
protected:
    static boost::shared_ptr<ContactJointHandlerInt> mContactJointHandlerImp;
    
    GenericSurfaceParameter* mSurfaceParameter;
};

DECLARE_CLASS(ContactJointHandler);

} //namespace oxygen

#endif // OXYGEN_CONTACTJOINTHANDLER_H
