/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: rendercontrol.cpp,v 1.5 2008/02/24 10:17:31 rollmark Exp $

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
#include "rendercontrol.h"
#include "customrender.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/sceneserver.h>

using namespace kerosin;
using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

RenderControl::RenderControl()
    : SimControlNode()
{
    mFramesRendered = 0;
}

RenderControl::~RenderControl()
{
}

void RenderControl::OnLink()
{
    RegisterCachedPath(mRenderServer, "/sys/server/render");

    if (mRenderServer.expired())
        {
            GetLog()->Error()
                << "(RenderControl) ERROR: RenderServer not found\n";
        }

    RegisterCachedPath(mOpenGLServer,"/sys/server/opengl");

    if (mOpenGLServer.expired())
        {
            GetLog()->Error()
                << "(RenderControl) ERROR: OpenGLServer not found\n";
        }
}

void
RenderControl::RenderCustom()
{
    // get list of registered CustomMonitor objects
    TLeafList customList;
    ListChildrenSupportingClass<CustomRender>(customList);

    for (
         TLeafList::iterator iter = customList.begin();
         iter != customList.end();
         ++iter
         )
    {
        shared_static_cast<CustomRender>((*iter))->Render();
    }
}

void RenderControl::EndCycle()
{
    if (
        (mOpenGLServer.expired()) ||
        (mRenderServer.expired())
        )
    {
        return;
    }

    // update the window (pumps event loop, etc..) and render the
    // current frame
    mOpenGLServer->Update();
    mRenderServer->Render();
    RenderCustom();
    mOpenGLServer->SwapBuffers();
    ++mFramesRendered;
}

int
RenderControl::GetFramesRendered() const
{
    return mFramesRendered;
}
