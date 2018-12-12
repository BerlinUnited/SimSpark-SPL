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
#ifndef KEROSIN_TEXTURE_H
#define KEROSIN_TEXTURE_H

#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <kerosin/kerosin_defines.h>
#include <kerosin/openglserver/openglwrapper.h>

namespace kerosin
{

class TextureServer;

/*      \class Texture

        This is the base class of all OpenGL based textures. In OpenGL a texture
        is represented by a so-called texture ID. This is a simple handle. The
        basic operations for creating/deleting this handle are contained in this
        class.

        Usually, textures are created via the texture server.

        NOTE:

        HISTORY:
                14.10.02 - MK
                        - Initial version

        TODO:
                - support mipmap building (currently done only when SGIS_generate_mipmap is supported)

        TOFIX:
*/
class KEROSIN_API Texture
{
    //
    // functions
    //
public:
    Texture(bool use_gl = false);
    virtual ~Texture();

    //! release the associated OpenGL texture
    void Reset();

    //! acquire an OpenGL texture handle (texture not loaded)
    void Acquire();

    /** bind the texture contained in this object to the corresponding
        target (user code is responsible for setting correct enables
        and tex units)
    */
    virtual void Bind() const = 0;

    /** enable the target associated with a texture type
        (e.g. GL_TEXTURE_2D)
    */
    virtual void Enable() const = 0;

    /** disable the target associated with a texture type
        (e.g. GL_TEXTURE_2D)
    */
    virtual void Disable() const = 0;

    virtual void Clamp() const = 0;
    virtual void ClampToEdge() const = 0;
    virtual void Repeat() const = 0;

    // accessors
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    //
    // members
    //
protected:
    //! OpenGL texture handle
    GLuint mTexID;
    //! width of texture
    unsigned int mWidth;
    //! height of texture
    unsigned int mHeight;
    //! flag if we want to access OpenGL
    bool mUseGL;
};

}

#endif //KEROSIN_TEXTURE_H
