/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sceneserver_c.cpp,v 1.2 2003/08/31 13:08:43 rollmark Exp $

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

   SceneServer

   The scene server manages displayable subtrees within the object
   hierarchy. Each subtree begins with a Scene node. The scene server
   knows which scene node is currently active and updates/displays
   that node (and its corresponding subtree).
*/

#include "sceneserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(createScene)
{
        if (in.size() == 1)
        {
                SceneServer *ss = static_cast<SceneServer*>(obj);
                ss->CreateScene(any_cast<char*>(in[0]));
        }
}

FUNCTION(setActiveScene)
{
        if (in.size() == 1)
        {
                SceneServer *ss = static_cast<SceneServer*>(obj);
                ss->SetActiveScene(any_cast<char*>(in[0]));
        }
}

void CLASS(SceneServer)::DefineClass()
{
        DEFINE_BASECLASS(zeitgeist/Leaf);
        DEFINE_FUNCTION(createScene);
        DEFINE_FUNCTION(setActiveScene);
}
