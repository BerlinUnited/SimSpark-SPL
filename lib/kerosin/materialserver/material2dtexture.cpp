/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: material2dtexture.cpp,v 1.3 2007/06/20 00:26:17 fruit Exp $

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
#include "material2dtexture.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <kerosin/textureserver/textureserver.h>
#include <kerosin/textureserver/texture.h>
#include <kerosin/openglserver/openglwrapper.h>
#include <kerosin/openglserver/openglserver.h>

using namespace zeitgeist;
using namespace kerosin;
using namespace boost;
using namespace std;

Material2DTexture::Material2DTexture() : MaterialSolid()
{
}

Material2DTexture::~Material2DTexture()
{
}

bool Material2DTexture::LoadTexture(const std::string& texName, shared_ptr<Texture>& store)
{
    shared_ptr<ScriptServer> scriptServer = GetCore()->GetScriptServer();
    shared_ptr<TextureServer> textureServer = shared_dynamic_cast<TextureServer>
        (GetCore()->Get("/sys/server/texture"));

    store.reset();

    if (textureServer.get() == 0)
        {
            GetLog()->Error()
                << "(Material2DTexture) ERROR: cannot find TextureServer\n";
            return false;
        }

    store = textureServer->GetTexture(texName);
    return (store.get() != 0);
}

bool Material2DTexture::SetDiffuseTexture(const std::string& texName)
{
    return LoadTexture(texName,mTexDiffuse);
}

bool Material2DTexture::SetNormalTexture(const std::string& texName)
{
    return LoadTexture(texName,mTexNormal);
}

bool Material2DTexture::SetSpecularTexture(const std::string& texName)
{
    return LoadTexture(texName,mTexSpecular);
}

void Material2DTexture::Bind()
{
    SetupMaterial();

    if (mTexDiffuse.get() != 0)
        {
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glEnable(GL_TEXTURE_2D);
            mTexDiffuse->Bind();
        }

    if (mTexNormal.get() != 0)
        {
            glActiveTextureARB(GL_TEXTURE1_ARB);
            glEnable(GL_TEXTURE_2D);
            mTexNormal->Bind();
        }

    if (mTexSpecular.get() != 0)
        {
            glActiveTextureARB(GL_TEXTURE2_ARB);
            glEnable(GL_TEXTURE_2D);
            mTexSpecular->Bind();
        }
}
