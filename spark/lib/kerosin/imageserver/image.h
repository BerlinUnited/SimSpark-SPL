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
#ifndef KEROSIN_IMAGE_H
#define KEROSIN_IMAGE_H

/*      Image - A Wrapper for the DevIL Library
                (or loading images by anything else)

        NOTE: Initial version by MK (11.07.01)
        TODO:
                - add RGB access
                - image creation

*/
#ifdef HAVE_CONFIG_H
#include <sparkconfig.h>
#endif
#ifdef HAVE_IL_IL_H
#include <il.h>
#endif
#include <kerosin/kerosin_defines.h>

namespace kerosin
{

class KEROSIN_API Image
{
public:
#ifdef HAVE_IL_IL_H
    typedef ILuint TImgUInt;
    typedef ILubyte TImgUChar;
#else
    typedef unsigned int TImgUInt;
    typedef unsigned char TImgUChar;
#endif

public:
    Image();
    virtual ~Image();

    //! this makes the image active
    void Bind() const;

    // image information
    //! @return the image width
    TImgUInt Width() const;
    //! @return the image height
    TImgUInt Height() const;
    /** Image depth information.
       @return 1 for 2d images, >1 for 3d images
    */
    TImgUInt Depth() const;
    //! @return bits per pixel
    TImgUInt BitsPP() const;
    //! @return bytes per pixel
    TImgUInt BytesPP() const;
    //! @return format of pixels
    TImgUInt Type() const;
    //! @return byte format of image
    TImgUInt Format() const;

    TImgUChar* Data();

    //! @return true if the format has an alpha channel
    bool HasAlpha() const;
    bool Create(int w, int h, int b, void* data = 0);

protected:
    //! the (DevIL) ID which this image is bound to
    TImgUInt mId;
};

} // namespace kerosin

#endif //KEROSIN_IMAGE_H
