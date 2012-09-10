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
#ifndef KEROSIN_MATERIAL2DTEXTURE_H
#define KEROSIN_MATERIAL2DTEXTURE_H

#include "materialsolid.h"
#include <kerosin/kerosin_defines.h>
#include <kerosin/openglserver/openglserver.h>

namespace kerosin
{
class Texture;
class OpenGLServer;

class KEROSIN_API Material2DTexture : public MaterialSolid
{
    //
    // Functions
    //
public:
    Material2DTexture();
    virtual ~Material2DTexture();

    virtual void OnLink();

    /** loads the diffuse texture */
    bool SetDiffuseTexture(const std::string& texName);

    //! @return the name of the set diffuse texture
    const std::string& GetDiffuseTextureName() const;

    //! @return true if there is a diffuse texture name and a texture
    bool HasDiffuseTexture() const;

    /** loads the normal texture */
    bool SetNormalTexture(const std::string& texName);

    //! @return the name of the set normal texture
    const std::string& GetNormalTextureName() const;

    //! @return true if there is a normal texture name and a texture
    bool HasNormalTexture() const;

    /** loads the specular texture */
    bool SetSpecularTexture(const std::string& texName);

    //! @return the name of the set specular texture
    const std::string& GetSpecularTextureName() const;

    //! @return true if there is a specular texture name and a texture
    bool HasSpecularTexture() const;

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
    //! the diffuse texture name
    std::string mTexDiffuseName;

    /** the diffuse texture */
    boost::shared_ptr<Texture> mTexDiffuse;

    //! the normal texture name
    std::string mTexNormalName;

    /** the normal texture */
    boost::shared_ptr<Texture> mTexNormal;

    //! the specular texture name
    std::string mTexSpecularName;

    /** the specular texture */
    boost::shared_ptr<Texture> mTexSpecular;

    /** a cached reference to the OpenGLServer */
    CachedPath<kerosin::OpenGLServer> mOpenGLServer;
};

DECLARE_CLASS(Material2DTexture);
};

#endif // KEROSIN_MATERIAL2DTEXTURE_H
