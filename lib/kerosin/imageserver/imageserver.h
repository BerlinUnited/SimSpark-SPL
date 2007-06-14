/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: imageserver.h,v 1.9.10.1 2007/06/14 16:26:55 jboedeck Exp $

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

#include <IL/il.h>
#include <zeitgeist/class.h>

namespace kerosin
{

class Image;

/*
  ImageServer - Global Interface For All Image-Related Functionality

  What the ImageServer does:
  - Load/Save images
  - Create images with different formats
  - Conversion between formats

  NOTE:

  HISTORY:
                14.07.01 - MK
                        - Initial version
                29.07.01 - MK
                        - Uses classserver
                29.08.01 - MK
                        - Doesn't use classserver anymore :(
                        - Switched to DevIL for image loading needs, since the task of supporting
                          all major formats would have been too time consuming
                        - Cleaned up the interface of the imageserver quite a bit
                11.10.01 - MK
                        - Made singleton functionality more secure
                02.10.02 - MK
                        - Moved to Kerosin

        TODO:
                - Image creation
                - Image conversion
                - Pixel-level access

        TOFIX:
*/

class ImageServer : public zeitgeist::Leaf
{
public:
    ImageServer();

    // load/save

    /** interpret the file with the filter associated with inExt */
    boost::shared_ptr<Image> Load(const std::string& inName,
                                  ILenum inType = IL_TYPE_UNKNOWN);

    /** interpret the file with the filter associated with inExt */
    bool Save(const boost::shared_ptr<Image> &inImage, const std::string& inName,
              ILenum inType = IL_TYPE_UNKNOWN);

private:
    /** some internal error checking */
    bool    HandleErrors(const std::string& context="");
};

DECLARE_CLASS(ImageServer);

} // namespace kerosin

#endif //KEROSIN_IMAGESERVER_H
