/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: world.h 56 2009-03-17 18:03:47Z hedayat $

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
#ifndef OXYGEN_ODESPACE_H
#define OXYGEN_ODESPACE_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/int/spaceint.h>

namespace oxygen
{

class OXYGEN_API ODESpace : public SpaceInt
{

public:
    typedef std::set<dSpaceID> TSpaceIdSet;

public:
    ODESpace();
    virtual ~ODESpace();

    dSpaceID GetODESpace() const;
    dJointGroupID GetODEJointGroup() const;
    void Collide();
    virtual void DestroyPhysicsObject();
    virtual dSpaceID GetParentSpaceID();
    bool IsGlobalSpace();
    void DisableInnerCollision(bool disable);
    bool GetDisableInnerCollision() const;
    
    dSpaceID mODESpace;

protected:
    //void collisionNearCallback (void *data, dGeomID obj1, dGeomID obj2);
    virtual void OnUnlink();
    virtual void OnLink();
    void Collide(dSpaceID space);
    void HandleCollide(dGeomID obj1, dGeomID obj2);
    void HandleSpaceCollide(dGeomID obj1, dGeomID obj2);
    virtual bool ConstructInternal();
    virtual void PostPhysicsUpdateInternal();
    void DestroySpaceObjects();

private:
    dJointGroupID mODEContactGroup;

private:
    static TSpaceIdSet gDisabledInnerCollisionSet;
};

DECLARE_CLASS(ODESpace);

}

#endif //OXYGEN_ODESPACE_H
