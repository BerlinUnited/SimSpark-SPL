/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: material2dtexture.h,v 1.2 2004/04/18 16:32:36 rollmark Exp $

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

#include "materialsolid.h"

namespace kerosin
{
class Texture;

class Material2DTexture : public MaterialSolid
{
    //
    // Functions
    //
public:
    Material2DTexture();
    virtual ~Material2DTexture();

    /** loads the diffuse texture */
    bool SetDiffuseTexture(const std::string& texName);

    /** loads the normal texture */
    bool SetNormalTexture(const std::string& texName);

    /** loads the specular texture */
    bool SetSpecularTexture(const std::string& texName);

    /** binds the managed material, i.e. sets all necessary OpenGL
        states */
    virtual void Bind();

protected:
    /** tries to load the texturee texName and stores a reference */
    bool LoadTexture(const std::string& texName,
                     boost::shared_ptr<Texture>& store);

    //
    // Members
    //
protected:
    /** the diffuse texture */
    boost::shared_ptr<Texture> mTexDiffuse;

    /** the normal texture */
    boost::shared_ptr<Texture> mTexNormal;

    /** the specular texture */
    boost::shared_ptr<Texture> mTexSpecular;
};

DECLARE_CLASS(Material2DTexture);
};

#endif // KEROSIN_MATERIAL2DTEXTURE_H
