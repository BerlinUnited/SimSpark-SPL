/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: texture2d.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "texture2d.h"
#include <kerosin/openglserver/openglwrapper.h>
#include <kerosin/imageserver/image.h>

using namespace kerosin;

Texture2D::Texture2D(bool use_gl) : Texture(use_gl)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::Bind() const
{
    if (mTexID != 0)
        {
            glBindTexture(GL_TEXTURE_2D, mTexID);
        }
}

void Texture2D::Enable() const
{
    glEnable(GL_TEXTURE_2D);
}

void Texture2D::Disable() const
{
    glDisable(GL_TEXTURE_2D);
}

void Texture2D::Create(boost::shared_ptr<Image> &image)
{
    mWidth          = image->Width();
    mHeight         = image->Height();

    Acquire();
    Bind();

    glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );
    if(image->HasAlpha())
        {
            glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA8, mWidth, mHeight, 0, image->Format(), image->Type(), image->Data());
        }
    else
        {
            glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGB8, mWidth, mHeight, 0, image->Format(), image->Type(), image->Data());
        }

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

void Texture2D::Clamp() const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void Texture2D::ClampToEdge() const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture2D::Repeat() const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
