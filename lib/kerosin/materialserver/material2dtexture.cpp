/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: material2dtexture.cpp,v 1.1 2004/03/20 12:53:10 rollmark Exp $

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
#include <kerosin/textureserver/textureserver.h>
#include <kerosin/textureserver/texture.h>
#include <kerosin/openglserver/openglserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace zeitgeist;
using namespace kerosin;
using namespace boost;
using namespace std;

Material2DTexture::Material2DTexture() : Material()
{
}

Material2DTexture::~Material2DTexture()
{
}

bool Material2DTexture::Load(const std::string& matName)
{
    shared_ptr<ScriptServer> scriptServer = GetCore()->GetScriptServer();
    shared_ptr<TextureServer> textureServer = shared_dynamic_cast<TextureServer>
        (GetCore()->Get("/sys/server/texture"));

    if (textureServer.get() == 0)
        {
            GetLog()->Error()
                << "(Material2DTexture) ERROR: cannot find TextureServer\n";
            return false;
        }

    if (
        (scriptServer.get() == 0) ||
        (! scriptServer->Run(matName))
        )
    {
        return false;
    }

    string diffuse, normal, specular;
    if(scriptServer->GetVariable("Material.Diffuse", diffuse) == false)
        {
            // backup - default diffuse texture
            diffuse = "sys/white.png";
        }
    mTexDiffuse = textureServer->GetTexture(diffuse);

    if(scriptServer->GetVariable("Material.Normal", normal) == false)
        {
            // backup - default normal texture
            normal = "sys/defaultNormal.png";
        }
    mTexNormal = textureServer->GetTexture(normal);

    if(scriptServer->GetVariable("Material.Specular", specular) == false)
        {
            // backup - default specular texture
            specular = diffuse;
        }
    mTexSpecular = textureServer->GetTexture(specular);

    return true;
}

void Material2DTexture::Bind()
{
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
