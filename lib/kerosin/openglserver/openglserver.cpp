/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: openglserver.cpp,v 1.11 2004/03/04 13:53:26 rollmark Exp $

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

#include "openglserver.h"
#include <SDL/SDL.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/logserver/logserver.h>

using namespace std;

namespace kerosin
{
  class MapHolder
  {
  public:
    //! this structure will be used to map program names to OpenGL IDs
#if HAVE_HASH_MAP
    typedef hash_map<string, unsigned int> TProgramCache;
#else
    typedef map<string, unsigned int> TProgramCache;
#endif
    TProgramCache mPrograms;
  };
}

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

OpenGLServer::OpenGLServer() : Leaf(), mExtensionReg(new GLExtensionReg()),
                               mWantsToQuit(false), mHolder( new MapHolder() )
{
}

OpenGLServer::~OpenGLServer()
{
  SDL_Quit();
}

boost::shared_ptr<GLExtensionReg> OpenGLServer::GetExtensionReg() const
{
  return mExtensionReg;
}

void OpenGLServer::Quit()
{
  mWantsToQuit = true;
}

bool OpenGLServer::WantsToQuit() const
{
  return mWantsToQuit;
}

void OpenGLServer::Update()
{
  // Our SDL event placeholder.
  SDL_Event event;

  // Grab all the events off the queue.
  while( SDL_PollEvent( &event ) )
    {
    }
}

void OpenGLServer::SwapBuffers() const
{
  SDL_GL_SwapBuffers();
}

unsigned int OpenGLServer::LoadARBProgram(GLenum /*target*/, const char* /*fileName*/)
{
#if 0
  // only try to load stuff if the extension is supported
  if (!mExtensionReg->Has_GL_ARB_vertex_program())
    {
      return 0;
    }

  // before actually loading, try the cache
  MapHolder::TProgramCache::iterator entry = mHolder->mPrograms.find(fileName);

  if (entry != mHolder->mPrograms.end())
    {
      // we already have a match
      return (*entry).second;
    }

  unsigned int id = 0;

  // open file
  shared_ptr<FileServer> fileServer = shared_static_cast<FileServer>(GetCore()->Get("/sys/server/file"));
  salt::RFile *file = fileServer->Open(fileName);

  if (!file) return 0;

  unsigned char *buffer = new unsigned char[file->Size()+1];
  file->Read(buffer, file->Size());

  glGenProgramsARB(1, &id);
  glBindProgramARB(target, id);

  // try to load the actual program
  glProgramStringARB(target, GL_PROGRAM_FORMAT_ASCII_ARB, file->Size(), buffer);

  // free memory
  delete file;
  delete []buffer;

  const unsigned char* error = glGetString(GL_PROGRAM_ERROR_STRING_ARB);

  // if an error occured, display error message
  if (error[0] != 0)
    {
      int i;
      glDeleteProgramsARB(1, &id);
      glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &i);
      GetCore()->GetLogServer()->Error() << "ERROR: Loading ARB program (Pos: " << i << ")..." << endl;
      GetCore()->GetLogServer()->Error() << "  => " << error << endl;
      return 0;
    }

  // enter program into the cache
  mHolder->mPrograms[fileName] = id;

  return id;
#else
  return 0;
#endif
}

unsigned int OpenGLServer::LoadARBVertexProgram(const char* fileName)
{
  // only try to load stuff if the extension is supported
  if (!mExtensionReg->Has_GL_ARB_vertex_program())
    {
      return 0;
    }

  return LoadARBProgram(GL_VERTEX_PROGRAM_ARB, fileName);
}

unsigned int OpenGLServer::LoadARBFragmentProgram(const char* /*fileName*/)
{
  // only try to load stuff if the extension is supported
  //      if (!mExtensionReg->Has_GL_ARB_fragment_program())
  //      {
  //              return 0;
  //      }

  //      return LoadARBProgram(GL_FRAGMENT_PROGRAM_ARB, fileName);
  return 0;
}

/*!
  Set up the OpenGL viewport, initialize extension registry
*/
bool OpenGLServer::ConstructInternal()
{
  int result = SDL_Init(SDL_INIT_VIDEO);

  if( result < 0 )
    {
      GetLog()->Error() << "ERROR: (OpenGLServer) Could not init SDL."
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
        << "(OpenGLServer) error reading config from ScriptServer\n";
      return false;
    }

  GetLog()->Normal() << "(OpenGLServer) bits per channel (RGB): "
                     << redBits << " "
                     << greenBits << " "
                     << blueBits << "\n";

  GetLog()->Normal() << "(OpenGLServer)"
                     << " depth bits= " << depthBits
                     << " alpha depth= " << alphaBits
                     << " stencil depth= " << stencilBits
                     << "\n";

  GetLog()->Normal() << "(OpenGLServer)"
                     << " doubleBuffer= " << doubleBuffer
                     << " fullScreen= " << fullScreen
                     << "\n";

  GetLog()->Normal() << "(OpenGLServer)"
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
        << "ERROR: (OpenGLServer) SDL_SetVideoMode() failed\n";
      return false;
    }

  SDL_ShowCursor(SDL_DISABLE);

  SDL_WM_SetCaption(title.c_str(), NULL);

  mExtensionReg->Init();

  // check if fancy lighting is supported
  mSupportsFancyLighting = true;

  //      if (!mExtensionReg->Has_GL_ARB_vertex_program() || !mExtensionReg->Has_GL_ARB_fragment_program())
  //      {
  GetLog()->Normal() << "WARNING: GL_ARB_vertex_program not supported. "
                     << "disabling fancy lighting\n" << endl;
  mSupportsFancyLighting = false;
  //      }

  glClear(GL_COLOR_BUFFER_BIT);

  GetLog()->Normal() << "(OpenGLServer) Initialized OpenGL Window\n";

  const unsigned char* glRenderer = glGetString(GL_RENDERER);
  const unsigned char* glVersion = glGetString(GL_VERSION);
  const unsigned char* glExtensions = glGetString(GL_EXTENSIONS);

  GetLog()->Normal() << "(OpenGLServer) GL_RENDERER:   " << glRenderer   << "\n";
  GetLog()->Normal() << "(OpenGLServer) GL_VERSION:    " << glVersion    << "\n";
  GetLog()->Normal() << "(OpenGLServer) GL_EXTENSIONS: " << glExtensions << "\n";

  return true;
}

void OpenGLServer::ToggleFancyLighting()
{
  if (mSupportsFancyLighting)
    mSupportsFancyLighting = false;
  else
    mSupportsFancyLighting = true;
}

