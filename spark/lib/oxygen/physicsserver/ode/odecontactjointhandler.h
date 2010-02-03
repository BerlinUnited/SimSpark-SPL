/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeobject.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#ifndef OXYGEN_ODECONTACTJOINTHANDLER_H
#define OXYGEN_ODECONTACTJOINTHANDLER_H

#include <oxygen/physicsserver/int/contactjointhandlerint.h>
#include <oxygen/physicsserver/ode/odecollider.h>

namespace oxygen
{

class OXYGEN_API ODEContactJointHandler : public ContactJointHandlerInt, public ODECollider
{
    /** \class ContactJointHandler is a CollisionHandler that creates a
        contact joint between the two bodies associated with the two
        affected collision geoms.
        
        See physicsserver/int/contactjointhandlerint.h for documentation. 
    */
    
public:
    ODEContactJointHandler();
    GenericSurfaceParameter* Initialize();
    long RetrieveBody(long geomID);
    long CreateContactJoint(long worldID, long jointGroupID, GenericContact& contact);
    void AttachContactJoint(long jointID, long bodyID1, long bodyID2);
    void CalcSurfaceParam(GenericContact& surface, 
                          GenericSurfaceParameter& collideeParam,
                          GenericSurfaceParameter* surfacePtr);
    int GetContactMode(GenericSurfaceParameter* surface) const;
    void SetContactBounceMode(bool set, GenericSurfaceParameter* surface);
    void SetBounceValue(float bounce, GenericSurfaceParameter* surface);
    float GetBounceValue(GenericSurfaceParameter* surface) const;
    void SetMinBounceVel(float vel, GenericSurfaceParameter* surface);
    float GetMinBounceVel(GenericSurfaceParameter* surface) const;
    void SetContactSoftERPMode(bool set, GenericSurfaceParameter* surface);
    void SetContactSoftERP(float erp, GenericSurfaceParameter* surface);
    float GetContactSoftERP(GenericSurfaceParameter* surface) const;
    void SetContactSoftCFMMode(bool set, GenericSurfaceParameter* surface);
    void SetContactSoftCFM(float cfm, GenericSurfaceParameter* surface);
    float GetContactSoftCFM(GenericSurfaceParameter* surface) const;
    void SetContactSlipMode (bool set, GenericSurfaceParameter* surface);
    void SetContactSlip(float slip, GenericSurfaceParameter* surface);
    float GetContactSlip1(GenericSurfaceParameter* surface) const;
    float GetContactSlip2(GenericSurfaceParameter* surface) const;
    void SetContactMu(float mu, GenericSurfaceParameter* surface);
    float GetContactMu(GenericSurfaceParameter* surface) const;
        
protected:    
    f_inline float MixValues(const float v1, const float v2, const int n) const;
    void CalcSurfaceParamInternal(dSurfaceParameters& surface,
                                  const dSurfaceParameters& collideeParam,
                                  const dSurfaceParameters* ODESurface);
    void SetContactMode(int mode, bool set, dSurfaceParameters* ODESurface);
};

} //namespace oxygen

#endif //OXYGEN_ODECONTACTJOINTHANDLER_H
