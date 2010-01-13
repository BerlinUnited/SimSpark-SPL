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
public:
    ODEContactJointHandler();
    float MixValues(const float v1, const float v2, const int n) const;
    void Initialize();
        
protected:
    /** the ODE surface parameters of the created contact joint */
    dSurfaceParameters mSurfaceParameter;
    
    void CalcSurfaceParam(dSurfaceParameters& surface,
                          const dSurfaceParameters& collideeParam);
};

} //namespace oxygen

#endif //OXYGEN_ODECONTACTJOINTHANDLER_H
