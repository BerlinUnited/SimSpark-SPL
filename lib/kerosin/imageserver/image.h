/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: image.h,v 1.7 2003/11/14 14:05:51 fruit Exp $

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

        NOTE:

        HISTORY:
                11.07.01 - MK
                        - Initial version
                28.08.01 - MK
                        - Added support for a palette
                29.08.01 - MK
                        - Rewrite for DevIL
                03.09.01 - MK
                        - OpenGL texture support

        TODO:
                - add RGB access
                - image creation


        TOFIX:
*/

#include <IL/il.h>

namespace kerosin
{

class Image
{
public:
    // constructor/destructor
    Image();
    virtual ~Image();

    // this makes the image active
    void    Bind();

    // image information
    ILuint  Width();                // width
    ILuint  Height();               // height
    ILuint  Depth();                // depth (==1 for 2d images, >1 for 3d images)

    ILuint  BitsPP();               // bits per pixel
    ILuint  BytesPP();              // bytes per pixel

    ILuint  Type();                 // format of pixels
    ILuint  Format();               // byte format of image

    ILubyte*Data();

    bool    HasAlpha();             // does the format have an alpha channel
    bool    Create(int w, int h, int b, void *data = NULL);

    // the interface functions ... these *have* to be implemented by derived classes
    //virtual bool Create() = 0;
    //virtual void SetPixel(int x, int y, long color) const = 0;
    //virtual long GetPixel(int x, int y) const = 0;

    /*
      virtual long MakeCol(int a, int r, int g, int b) const = 0;
      virtual void GetCol(long col, int& a, int& r, int& g, int& b) const = 0;
      virtual int      GetA(long col) const = 0;
      virtual int      GetR(long col) const = 0;
      virtual int      GetG(long col) const = 0;
      virtual int      GetB(long col) const = 0;

      // accessors
      f_inline void SetWidth (int inWidth)    {       mWidth  = inWidth;      }
      f_inline void SetHeight(int inHeight)   {       mHeight = inHeight;     }
    */
protected:
    ILuint          mId;                    // the DevIL ID which this image is bound to
};

} // namespace kerosin

#endif //KEROSIN_IMAGE_H
