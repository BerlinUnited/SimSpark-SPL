/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: material2dtexture.h,v 1.1 2004/03/20 12:53:10 rollmark Exp $

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
#ifndef KEROSIN_MATERIAL2DTEXTURE_H
#define KEROSIN_MATERIAL2DTEXTURE_H

#include "material.h"

namespace kerosin
{
class Texture;

class Material2DTexture : public Material
{
    //
    // Functions
    //
public:
    Material2DTexture();
    virtual ~Material2DTexture();

    /** loads a set of textures. It runs a script matName that should
        set three variables named Material.Diffuse, Material.Normal
        and Material.Specular giving the names of the textures to load
     */
    bool Load(const std::string& matName);

    virtual void Bind();

    //
    // Members
    //
protected:
    boost::shared_ptr<Texture> mTexDiffuse;
    boost::shared_ptr<Texture> mTexNormal;
    boost::shared_ptr<Texture> mTexSpecular;
};

DECLARE_CLASS(Material2DTexture);
};

#endif // KEROSIN_MATERIAL2DTEXTURE_H
