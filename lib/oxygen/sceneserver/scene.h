/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: scene.h,v 1.2 2003/08/31 13:08:43 rollmark Exp $

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

#ifndef SCENE_H__
#define SCENE_H__

#include "basenode.h"

namespace kerosin
{

/** Scene is the root node of a simulatable/displayable hierarchy. It is
    usually created via the scene server.
*/
class Scene : public BaseNode
{
public:
        Scene();
        ~Scene();

        /** returns the world transform of this node (always identity, terminates upward recursion) */
        virtual const salt::Matrix&     GetWorldTransform()     const;
};

DECLARE_CLASS(Scene);

} //namespace kerosin
#endif //SCENE_H__
