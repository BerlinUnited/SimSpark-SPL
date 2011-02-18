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

   Scene


        NOTE:

        HISTORY:
                05.11.02 - MK
                        - Initial version

        TODO:

        TOFIX:
*/
#ifndef OXYGEN_SCENE_H
#define OXYGEN_SCENE_H

#include <oxygen/oxygen_defines.h>
#include "basenode.h"

namespace oxygen
{

/** Scene is the root node of a simulatable/displayable hierarchy. It is
    usually created via the scene server.
*/
class OXYGEN_API Scene : public BaseNode
{
public:
    Scene();
    ~Scene();

    /** returns the world transform of this node (always identity,
        terminates upward recursion) */
    virtual const salt::Matrix&     GetWorldTransform()     const;

    /** sets the world transform of this node */
    virtual void SetWorldTransform(const salt::Matrix &transform);

    /** marks the scene as modified, i.e. scene nodes were added or
        removed since the last update. This useful for monitors to
        decide between an incremental or a full state update
     */
    void SetModified(bool modified);

    /** returns true iff the scene is marked modified */
    bool GetModified();

    /** return how many times the scene was modified */
    int GetModifiedNum();

    /** return he modification count at the last update */
    int GetLastCacheUpdate();
protected:
    void UpdateCacheInternal();

    /** true, if the scene is modified */
    bool mModified;

    /** how many times the scene was modified */
    int mModifiedNum;
    
    /** the modification count at the last update */
    int mLastCacheUpdate;
};

DECLARE_CLASS(Scene);

} //namespace oxygen
#endif //OXYGEN_SCENE_H
