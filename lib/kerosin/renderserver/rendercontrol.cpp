/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: rendercontrol.cpp,v 1.1 2004/04/25 16:57:34 rollmark Exp $

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
#include <zeitgeist/logserver/logserver.h>
#include <kerosin/renderserver/renderserver.h>
#include <kerosin/openglserver/openglserver.h>

using namespace kerosin;
using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

RenderControl::RenderControl()
{
    mFramesRendered = 0;
}

RenderControl::~RenderControl()
{
}

void RenderControl::OnLink()
{
    mRenderServer = shared_dynamic_cast<RenderServer>
        (GetCore()->Get("/sys/server/render"));

    if (mRenderServer.get() == 0)
        {
            GetLog()->Error()
                << "(RenderControl) ERROR: RenderServer not found\n";
        }

    mOpenGLServer =  shared_dynamic_cast<OpenGLServer>
        (GetCore()->Get("/sys/server/opengl"));

    if (mOpenGLServer.get() == 0)
        {
            GetLog()->Error()
                << "(RenderControl) ERROR: OpenGLServer not found\n";
        }
}

void RenderControl::OnUnlink()
{
    mRenderServer.reset();
    mOpenGLServer.reset();
}

void RenderControl::EndCycle()
{
    // update the window (pumps event loop, etc..) and render the
    // current frame
    mOpenGLServer->Update();
    mRenderServer->Render();
    mOpenGLServer->SwapBuffers();
    ++mFramesRendered;
}

int RenderControl::GetFramesRendered()
{
    return mFramesRendered;
}





