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
#ifndef OXYGEN_CONTROLASPECT_H
#define OXYGEN_CONTROLASPECT_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/sceneserver/basenode.h>

namespace oxygen
{
class Scene;

class OXYGEN_API ControlAspect : public BaseNode
{
public:
    ControlAspect();
    virtual ~ControlAspect();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.

        \param time is the time passed since the last update in
        seconds
     */
    virtual void Update(float deltaTime) = 0;

    /** queries the SceneServer for the currently active scene node */
    boost::shared_ptr<Scene> GetActiveScene();

    /** returns a reference to a ControlAspect registered to the
        GameControlServer */
    void GetControlAspect(zeitgeist::Core::CachedLeafPath& aspect, const std::string& name);
};

DECLARE_ABSTRACTCLASS(ControlAspect);

} // namespace oxygen

#endif //OXYGEN_CONTROLASPECT_H
