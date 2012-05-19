/* -*- mode: c++= 0; c-basic-offset: 4= 0; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

   This program is free software= 0; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation= 0; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY= 0; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program= 0; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef OXYGEN_CONTACTJOINTHANDLERINT_H
#define OXYGEN_CONTACTJOINTHANDLERINT_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{

class OXYGEN_API ContactJointHandlerInt
{
public:
    virtual ~ContactJointHandlerInt() {}

    /** Returns a pointer to a SurfaceParameter class whose values were initialized
        with default values that make sense for most physics simulations.
    */
    virtual GenericSurfaceParameter* Initialize() = 0;

    /** Finds and returns the ID of the body that the collider specified by
        \param geomID has been registered to. */
    virtual long RetrieveBody(long geomID) = 0;

    /** Creates a contact joint in the world specified by \param worldID, within the joint
        group specified by \param jointGroupID
    */
    virtual long CreateContactJoint(long worldID, long jointGroupID, GenericContact& contact) = 0;

    /** Attaches the contact joint specified by \param jointID to the two bodies
        specified by \param body1 and \param body2
    */
    virtual void AttachContactJoint(long jointID, long bodyID1, long bodyID2) = 0;

    /** Calculates the surface parameters to take into account for a collision
        by factoring in the surface parameters of the two colliding objects,
        specified by \param colideeParam and \param surfacePtr
    */
    virtual void CalcSurfaceParam(GenericContact& surface,
                                  GenericSurfaceParameter& collideeParam,
                                  GenericSurfaceParameter* surfacePtr) = 0;

    /** returns the current set of contact mode flags in the surface
        parameter
     */
    virtual int GetContactMode(GenericSurfaceParameter* surface) const = 0;

    /** sets or resets the Bounce mode flag */
    virtual void SetContactBounceMode(bool set, GenericSurfaceParameter* surface) = 0;

    /** sets the bounce value */
    virtual void SetBounceValue(float bounce, GenericSurfaceParameter* surface) = 0;

    /** returns the bounce value */
    virtual float GetBounceValue(GenericSurfaceParameter* surface) const = 0;

    /** sets the mininum incoming velocity necessary for bounce */
    virtual void SetMinBounceVel(float vel, GenericSurfaceParameter* surface) = 0;

    /** returns the mininum incoming velocity necessary for bounce */
    virtual float GetMinBounceVel(GenericSurfaceParameter* surface) const = 0;

    /** sets or resets the error reduction parameter (ERP) mode,
        useful to make surfaces soft.
    */
    virtual void SetContactSoftERPMode(bool set, GenericSurfaceParameter* surface) = 0;

    /** sets the contact normal error reduction parameter (ERP) */
    virtual void SetContactSoftERP(float erp, GenericSurfaceParameter* surface) = 0;

    /** returns the contact normal error reduction parameter (ERP) */
    virtual float GetContactSoftERP(GenericSurfaceParameter* surface) const = 0;

    /** sets or resets the constraint force mixing mode (CFM), useful
        to make surfaces soft
    */
    virtual void SetContactSoftCFMMode(bool set, GenericSurfaceParameter* surface) = 0;

    /** sets the constraint force mixing parameter (CFM) */
    virtual void SetContactSoftCFM(float cfm, GenericSurfaceParameter* surface) = 0;

    /** returns the constraint force mixing parameter (CFM) */
    virtual float GetContactSoftCFM(GenericSurfaceParameter* surface) const = 0;

    /** sets or resets the force dependent contact slip mode (FDS) */
    virtual void SetContactSlipMode (bool set, GenericSurfaceParameter* surface) = 0;

    /** sets the force dependent slip (FDS) in both friction
        directions
    */
    virtual void SetContactSlip(float slip, GenericSurfaceParameter* surface) = 0;

    /** returns the force dependent slip in the first slip direction
        (FDS)
    */
    virtual float GetContactSlip1(GenericSurfaceParameter* surface) const = 0;

    /** returns the force dependent slip in the second slip direction
        (FDS)
    */
    virtual float GetContactSlip2(GenericSurfaceParameter* surface) const = 0;

    /** sets the Coulomb friction coefficient */
    virtual void SetContactMu(float mu, GenericSurfaceParameter* surface) = 0;

    /** returns the Coulomb friction coefficient */
    virtual float GetContactMu(GenericSurfaceParameter* surface) const = 0;

protected:
    /** returns \param v1 for \param n =1, \param v2 for /param n =2, and
        (\param v1 + \param v2) * 0.5 for \param n =3. Undefined for all
        other values of n.
    */
    virtual float MixValues(const float v1, const float v2, const int n) const = 0;
};

} //namespace oxygen

#endif //OXYGEN_CONTACTJOINTHANDLERINT_H
