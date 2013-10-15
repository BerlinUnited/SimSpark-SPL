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
#ifndef KEROSIN_IMAGESERVER_H
#define KEROSIN_IMAGESERVER_H

#ifdef HAVE_CONFIG_H
#include <sparkconfig.h>
#endif
#ifdef HAVE_IL_IL_H
#include <il.h>
#ifndef IL_ILBM
#define IL_ILBM IL_LBM
#endif
#endif

#include <zeitgeist/class.h>
#include <kerosin/kerosin_defines.h>

namespace kerosin
{

class Image;

/*
  ImageServer - Global Interface For All Image-Related Functionality

  What the ImageServer does:
  - Load/Save images
  - Create images with different formats
  - Conversion between formats

  NOTE: Initial version 14.07.01 - MK

  TODO:
  - Image creation
  - Image conversion
  - Pixel-level access
*/

class KEROSIN_API ImageServer : public zeitgeist::Leaf
{
public:
#ifdef HAVE_IL_IL_H
    enum EImgType
    {
        eTYPE_UNKNOWN = IL_TYPE_UNKNOWN,
        eBMP = IL_BMP,
        eCUT = IL_CUT,
        eDOOM = IL_DOOM,
        eDOOM_FLAT = IL_DOOM_FLAT,
        eICO = IL_ICO,
        eJPG = IL_JPG,
        eJFIF = IL_JFIF,
        eLBM = IL_ILBM,
        ePCD = IL_PCD,
        ePCX = IL_PCX,
        ePIC = IL_PIC,
        ePNG = IL_PNG,
        ePNM = IL_PNM,
        eSGI = IL_SGI,
        eTGA = IL_TGA,
        eTIF = IL_TIF,
        eCHEAD = IL_CHEAD,
        eRAW = IL_RAW,
        eMDL = IL_MDL,
        eWAL = IL_WAL,
        eLIF = IL_LIF,
        eMNG = IL_MNG,
        eJNG = IL_JNG,
        eGIF = IL_GIF,
        eDDS = IL_DDS,
        eDCX = IL_DCX,
        ePSD = IL_PSD,
        eEXIF = IL_EXIF,
        ePSP = IL_PSP,
        ePIX = IL_PIX,
        ePXR = IL_PXR,
        eXPM = IL_XPM,
        eHDR = IL_HDR,
        eJASC_PAL = IL_JASC_PAL
    };
#else
    enum EImgType
    {
        eTYPE_UNKNOWN, eBMP,          eCUT,          eDOOM,
        eDOOM_FLAT,    eICO,          eJPG,          eJFIF,
        eLBM,          ePCD,          ePCX,          ePIC,
        ePNG,          ePNM,          eSGI,          eTGA,
        eTIF,          eCHEAD,        eRAW,          eMDL,
        eWAL,          eLIF,          eMNG,          eJNG,
        eGIF,          eDDS,          eDCX,          ePSD,
        eEXIF,         ePSP,          ePIX,          ePXR,
        eXPM,          eHDR,          eJASC_PAL
    };
#endif
public:
    ImageServer();

    // load/save

    /** Load the file with the filter associated with the given type.
        If inType is eTYPE_UNKNOWN, then Load try to find a handler by the
        file extension provided. If using DevIL, this behavior is done
        automatically by the library (Without DevIL, loading and saving
        is disabled at the moment).
       @param inName the file name of the image
       @param inType hint for the file type
       @return a boost::shared_ptr to the Image (handle)
    */
    boost::shared_ptr<Image> Load(const std::string& inName,
                                  EImgType inType = eTYPE_UNKNOWN) const;

private:
    /** Some internal error checking.
        This routine checks for DevIL errors and logs them.
       @returns true if an error has occured and false if not.
    */
    bool HandleErrors(const std::string& fileName) const;
};

DECLARE_CLASS(ImageServer);

} // namespace kerosin

#endif //KEROSIN_IMAGESERVER_H
