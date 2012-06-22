/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of simspark
   Tue May 9 2006
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2007 RoboCup Soccer Server 3D Maintenance Group
   $Id: baserenderserver.cpp 3 2008-11-21 02:38:08Z hedayat $

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

   BaseRenderServer

   NOTE: This is an abstract renderserver in order to support different render servers

   HISTORY:
       29/03/07 - OO  - Initial version

*/
#include "baserenderserver.h"
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;

void
BaseRenderServer::OnLink()
{
    // setup SceneServer reference
    RegisterCachedPath(mSceneServer, "/sys/server/scene");

    if (mSceneServer.expired())
    {
        GetLog()->Error() << "(BaseRenderServer) ERROR: SceneServer not found\n";
    }
}

void
BaseRenderServer::OnUnlink()
{
    mActiveScene.reset();
    Leaf::OnUnlink();
}

bool
BaseRenderServer::GetActiveScene()
{
    if (mSceneServer.expired())
    {
        mActiveScene.reset();
    }

    mActiveScene = mSceneServer->GetActiveScene();

    if (mActiveScene.get() == 0)
    {
        GetLog()->Error() << "(BaseRenderServer) ERROR: found no active scene\n";
        return false;
    }

    return true;
}

void 
BaseRenderServer::UpdateCached()
{
    mActiveScene.reset();
}
