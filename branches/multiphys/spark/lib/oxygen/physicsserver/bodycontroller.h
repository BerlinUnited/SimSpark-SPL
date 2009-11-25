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
#ifndef OXYGEN_BODYCONTROLLER_H
#define OXYGEN_BODYCONTROLLER_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/sceneserver/basenode.h>

namespace oxygen
{
class RigidBody;

/** \class BodyController is a baseclass for nodes that control the
    motion of an associated body.
*/
class OXYGEN_API BodyController : public BaseNode
{
public:
    BodyController() : BaseNode() {}
    virtual ~BodyController() {}

    /** update variables from a script */
    virtual void UpdateCached();

protected:
    /** sets up the reference to the controlled body */
    virtual void OnLink();

    /** resets the reference to the controlled body */
    virtual void OnUnlink();

protected:
    /** the reference to the controlled body */
    boost::shared_ptr<RigidBody> mBody;
};

DECLARE_CLASS(BodyController);

} // namespace oxygen

#endif // OXYGEN_BODYCONTROLLER_H
