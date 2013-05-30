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

#include "openglsystemsdl.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <kerosin/openglserver/openglwrapper.h>
#ifndef WIN32
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

using namespace std;
using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

OpenGLSystemSDL *gInputSystem;

OpenGLSystemSDL::OpenGLSystemSDL() : OpenGLSystem(), mWantsToQuit(false)
{
}

OpenGLSystemSDL::~OpenGLSystemSDL()
{
    SDL_Quit();
}

void OpenGLSystemSDL::Update()
{
    // Our SDL event placeholder.
    SDL_Event event;

    // Grab all the events off the queue.
    while( SDL_PollEvent( &event ) )
        {
            if (event.type == SDL_QUIT)
                mWantsToQuit = true;
        }
}

void OpenGLSystemSDL::SwapBuffers()
{
    SDL_GL_SwapBuffers();
}

bool OpenGLSystemSDL::Init()
{
    int result = SDL_Init(SDL_INIT_VIDEO);

    if( result < 0 )
        {
            GetLog()->Error() << "ERROR: (OpenGLSystemSDL) Could not init SDL."
                              << "SDL_Init returned error "
                              << result << "\n";
            return false;
        }

    const SDL_VideoInfo* info = SDL_GetVideoInfo();

    int redBits;
    int greenBits;
    int blueBits;
    int alphaBits;
    int depthBits;
    int stencilBits;
    bool doubleBuffer;
    bool fullScreen;
    int xRes, yRes;
    string title;

    bool getConfig =
        (
         GetScript()->GetVariable("Viewport.RedBits", redBits) &&
         GetScript()->GetVariable("Viewport.GreenBits", greenBits) &&
         GetScript()->GetVariable("Viewport.BlueBits", blueBits) &&
         GetScript()->GetVariable("Viewport.AlphaBits", alphaBits) &&
         GetScript()->GetVariable("Viewport.DepthBits", depthBits) &&
         GetScript()->GetVariable("Viewport.StencilBits", stencilBits) &&
         GetScript()->GetVariable("Viewport.DoubleBuffer", doubleBuffer) &&
         GetScript()->GetVariable("Viewport.FullScreen", fullScreen) &&
         GetScript()->GetVariable("Viewport.XRes", xRes) &&
         GetScript()->GetVariable("Viewport.YRes", yRes) &&
         GetScript()->GetVariable("Application.Title", title)
         );

    if (! getConfig)
        {
            GetLog()->Error()
                << "(OpenGLSystemSDL) error reading config from ScriptServer\n";
            return false;
        }

    GetLog()->Debug() << "(OpenGLSystemSDL) bits per channel (RGB): "
                       << redBits << " "
                       << greenBits << " "
                       << blueBits << "\n";

    GetLog()->Debug() << "(OpenGLSystemSDL)"
                       << " depth bits= " << depthBits
                       << " alpha depth= " << alphaBits
                       << " stencil depth= " << stencilBits
                       << "\n";

    GetLog()->Debug() << "(OpenGLSystemSDL)"
                       << " doubleBuffer= " << doubleBuffer
                       << " fullScreen= " << fullScreen
                       << "\n";

    GetLog()->Debug() << "(OpenGLSystemSDL)"
                       << " xRes = " << xRes
                       << " yRes = " << yRes
                       << "\n";

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, redBits);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, greenBits);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, blueBits);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, alphaBits);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depthBits);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencilBits);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, doubleBuffer ? 1:0);

    int flags = SDL_OPENGL;
    if (fullScreen)
        {
            flags |= SDL_FULLSCREEN;
        }

    SDL_Surface *screen = SDL_SetVideoMode
        (xRes, yRes, info->vfmt->BitsPerPixel, flags);

    if (screen == 0)
        {
            GetLog()->Error()
                << "ERROR: (OpenGLSystemSDL) SDL_SetVideoMode() failed\n";
            return false;
        }

    SDL_WarpMouse(xRes/2,yRes/2);
    SDL_WM_SetCaption(title.c_str(), NULL);

    glClear(GL_COLOR_BUFFER_BIT);

    GetLog()->Normal() << "(OpenGLSystemSDL) Initialized OpenGL Window\n";

    const unsigned char* glRenderer = glGetString(GL_RENDERER);
    const unsigned char* glVersion = glGetString(GL_VERSION);
    const unsigned char* glExtensions = glGetString(GL_EXTENSIONS);

    GetLog()->Debug() << "(OpenGLSystemSDL) GL_RENDERER:   " << glRenderer   << "\n";
    GetLog()->Debug() << "(OpenGLSystemSDL) GL_VERSION:    " << glVersion    << "\n";
    GetLog()->Debug() << "(OpenGLSystemSDL) GL_EXTENSIONS: " << glExtensions << "\n";
    GetLog()->Debug() << "(OpenGLSystemSDL) GL_MAX_LIGHTS: " << GL_MAX_LIGHTS << "\n";

    return true;
}

bool OpenGLSystemSDL::WantsToQuit() const
{
    return mWantsToQuit;
}
