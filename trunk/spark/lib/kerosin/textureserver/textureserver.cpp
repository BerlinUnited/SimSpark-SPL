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

#include "textureserver.h"
#include <zeitgeist/logserver/logserver.h>
#include "../openglserver/openglserver.h"
#include "texture2d.h"
#include "../imageserver/imageserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

TextureServer::TextureServer() : Leaf()
{
}

TextureServer::~TextureServer()
{
}

void TextureServer::OnLink()
{
    // setup ImageServer reference
    RegisterCachedPath(mImageServer, "/sys/server/image");

    if (mImageServer.expired())
        {
            GetLog()->Error()
                << "(TextureServer) ERROR: ImageServer not found\n";
        }

    // setup OpenGLServer reference
    RegisterCachedPath(mOpenGLServer, "/sys/server/opengl");
}

boost::shared_ptr<Texture> TextureServer::GetTexture(const std::string &name)
{
    TTextureCache::iterator entry = mTextureCache.find(name);

    if (entry != mTextureCache.end())
    {
        // we already have a match
        return (*entry).second;
    }

    if (mImageServer.expired())
    {
        return shared_ptr<Texture>();
    }

    // no match for that name, so we have to load it
    shared_ptr<Image> image = mImageServer->Load(name.c_str());

    if (! image.get())
    {
        return shared_ptr<Texture>();
    }

    bool use_gl = false;
    if (!mOpenGLServer.expired())
    {
        use_gl = !mOpenGLServer->IsGLLocked();
    }

    Texture2D *tex2D = new Texture2D(use_gl);
    tex2D->Create(image);
    shared_ptr<Texture> texture(tex2D);

    // register the texture, so we will find it later
    mTextureCache[name] = texture;

    return texture;
}
