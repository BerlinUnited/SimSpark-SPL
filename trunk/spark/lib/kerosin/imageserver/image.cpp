/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of simspark
   Mon May 9 2005
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: image.cpp,v 1.3 2008/02/22 16:48:18 hedayat Exp $

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
#include "image.h"

using namespace kerosin;

// constructor
Image::Image()
{
#ifdef HAVE_IL_IL_H
    // let's create a DevIL ID for this image
    ilGenImages(1, &mId);
#endif
}

// destructor
Image::~Image()
{
#ifdef HAVE_IL_IL_H
    // free the image with DevIL
    ilDeleteImages(1, &mId);
#endif
}

void
Image::Bind() const
{
#ifdef HAVE_IL_IL_H
    ilBindImage(mId);
#endif
}

Image::TImgUInt
Image::Width() const
{
#ifdef HAVE_IL_IL_H
    Bind();
    return ilGetInteger(IL_IMAGE_WIDTH);
#else
    return 0;
#endif
}

Image::TImgUInt
Image::Height() const
{
#ifdef HAVE_IL_IL_H
    Bind();
    return ilGetInteger(IL_IMAGE_HEIGHT);
#else
    return 0;
#endif
}

Image::TImgUInt
Image::Depth() const
{
#ifdef HAVE_IL_IL_H
    Bind();
    return ilGetInteger(IL_IMAGE_DEPTH);
#else
    return 0;
#endif
}

Image::TImgUInt
Image::BitsPP() const
{
#ifdef HAVE_IL_IL_H
    Bind();
    return ilGetInteger(IL_IMAGE_BITS_PER_PIXEL );
#else
    return 0;
#endif
}

Image::TImgUInt
Image::BytesPP() const
{
#ifdef HAVE_IL_IL_H
    Bind();
    return ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL );
#else
    return 0;
#endif
}

Image::TImgUInt
Image::Type() const
{
#ifdef HAVE_IL_IL_H
    Bind();
    return ilGetInteger(IL_IMAGE_TYPE);
#else
    return 0;
#endif
}

Image::TImgUInt
Image::Format() const
{
#ifdef HAVE_IL_IL_H
    Bind();
    return ilGetInteger(IL_IMAGE_FORMAT);
#else
    return 0;
#endif
}

Image::TImgUChar*
Image::Data()
{
#ifdef HAVE_IL_IL_H
    Bind();
    return ilGetData();
#else
    return 0;
#endif
}

bool
Image::HasAlpha() const
{
#ifdef HAVE_IL_IL_H
    Bind();
    TImgUInt format = Format();

    switch(format)
    {
    case IL_RGB:
    case IL_BGR:
        return false;
        break;
    case IL_RGBA:
    case IL_BGRA:
        return true;
        break;
    default:
        return false;
    }
#else
    return false;
#endif
}


bool
Image::Create(int w, int h, int b, void* data)
{
#ifdef HAVE_IL_IL_H
    Bind();
    if (b==3)
    {
        ilTexImage(w, h, 1, b, IL_RGB, IL_UNSIGNED_BYTE, data);
    }
    else
    {
        ilTexImage(w, h, 1, b, IL_RGBA, IL_UNSIGNED_BYTE, data);
    }
    return true;
#else
    return false;
#endif
}
