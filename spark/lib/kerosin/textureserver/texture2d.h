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
#ifndef KEROSIN_TEXTURE2D_H
#define KEROSIN_TEXTURE2D_H

#include <kerosin/kerosin_defines.h>
#include "texture.h"

namespace kerosin
{

class Image;

class KEROSIN_API Texture2D : public Texture
{
    //
    // functions
    //
public:
    Texture2D(bool use_gl = false);
    ~Texture2D();

    /** bind the texture contained in this object to the corresponding
        target (user code is responsible for setting correct enables
        and tex units)
    */
    virtual void Bind() const;

    /** enable the target associated with a texture type
        (e.g. GL_TEXTURE_2D)
    */
    virtual void Enable() const;

    /** disable the target associated with a texture type
        (e.g. GL_TEXTURE_2D)
    */
    virtual void Disable() const;

    virtual void Clamp() const;
    virtual void ClampToEdge() const;
    virtual void Repeat() const;

    void Create(boost::shared_ptr<Image> &image);
};

} //kerosin

#endif //KEROSIN_TEXTURE2D_H
