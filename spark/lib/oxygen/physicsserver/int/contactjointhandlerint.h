/* -*- mode: c++= 0; c-basic-offset: 4= 0; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: space.h 102 2009-11-18 07:24:29Z a-held $

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

#include <oxygen/physicsserver/ode/odewrapper.h>

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{

class OXYGEN_API ContactJointHandlerInt
{
public:
    virtual float MixValues(const float v1, const float v2, const int n) const = 0;
    virtual GenericSurfaceParameter* Initialize() = 0;
    virtual long RetrieveBody(long geomID) = 0;
    virtual long CreateContactJoint(long worldID, long jointGroupID, GenericContact& contact) = 0;
    virtual void AttachContactJoint(long jointID, long bodyID1, long bodyID2) = 0;
    virtual void CalcSurfaceParam(GenericContact& surface, 
                                  GenericSurfaceParameter& collideeParam,
                                  GenericSurfaceParameter* surfacePtr) = 0;
    virtual int GetContactMode(GenericSurfaceParameter* surface) const = 0;
    virtual void SetContactBounceMode(bool set, GenericSurfaceParameter* surface) = 0;
    virtual void SetBounceValue(float bounce, GenericSurfaceParameter* surface) = 0;
    virtual float GetBounceValue(GenericSurfaceParameter* surface) const = 0;
    virtual void SetMinBounceVel(float vel, GenericSurfaceParameter* surface) = 0;
    virtual float GetMinBounceVel(GenericSurfaceParameter* surface) const = 0;
    virtual void SetContactSoftERPMode(bool set, GenericSurfaceParameter* surface) = 0;
    virtual void SetContactSoftERP(float erp, GenericSurfaceParameter* surface) = 0;
    virtual float GetContactSoftERP(GenericSurfaceParameter* surface) const = 0;
    virtual void SetContactSoftCFMMode(bool set, GenericSurfaceParameter* surface) = 0;
    virtual void SetContactSoftCFM(float cfm, GenericSurfaceParameter* surface) = 0;
    virtual float GetContactSoftCFM(GenericSurfaceParameter* surface) const = 0;
    virtual void SetContactSlipMode (bool set, GenericSurfaceParameter* surface) = 0;
    virtual void SetContactSlip(float slip, GenericSurfaceParameter* surface) = 0;
    virtual float GetContactSlip1(GenericSurfaceParameter* surface) const = 0;
    virtual float GetContactSlip2(GenericSurfaceParameter* surface) const = 0;
    virtual void SetContactMu(float mu, GenericSurfaceParameter* surface) = 0;
    virtual float GetContactMu(GenericSurfaceParameter* surface) const = 0;
};

} //namespace oxygen

#endif //OXYGEN_CONTACTJOINTHANDLERINT_H
