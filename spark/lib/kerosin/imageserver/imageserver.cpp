/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#include "imageserver.h"
#include "image.h"
#include <salt/fileclasses.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/logserver/logserver.h>
#ifndef Q_MOC_RUN
#include <boost/scoped_ptr.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/scoped_array.hpp>
#endif

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;
using namespace std;

boost::shared_ptr<FileServer> gFileServer;

//------------------------------------------------------------------------------------------------
// ImageServer implementation
//------------------------------------------------------------------------------------------------

// constructor
ImageServer::ImageServer()
    : Leaf()
{
#if HAVE_IL_IL_H
    // initialize DevIL
    ilInit();

    // and setup the default behavior
    // (this might come out of a config file at a later point)
    ilEnable(IL_FILE_OVERWRITE);
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
#else
#warning ======================================================================
#warning The ImageServer is will not work properly without using DevIL
#warning ======================================================================
#endif
}

//
// This function loads the file inName. If inType is eTYPE_UNKNOWN,
// then the library will try to find a handler by the file extension provided.
// This behavior is done automatically by the library!
//
boost::shared_ptr<Image>
ImageServer::Load(const string& inName, ImageServer::EImgType inType) const
{
    // lookup the file server
    boost::shared_ptr<FileServer> fileServer
        = static_pointer_cast<FileServer>(GetCore()->Get("/sys/server/file"));

    if (fileServer.get() == 0)
    {
        return boost::shared_ptr<Image>();
    }

    // create a new image
    boost::shared_ptr<Image> image(new Image());

    // make it active with DevIL
    image->Bind();

    boost::shared_ptr<RFile> rfile = fileServer->OpenResource(inName);

    scoped_array<unsigned char> buffer(new unsigned char[rfile->Size()]);
    rfile->Read(buffer.get(), rfile->Size());
    ilLoadL(inType, buffer.get(), rfile->Size());

    // check for errors
    if (HandleErrors(inName) == true)
    {
        // release the image and return
        return boost::shared_ptr<Image>();
    }

    return image;
}

// This routine checks for DevIL errors and logs them. The function returns
// 'true' if an error has occured and 'false' if not.
bool
ImageServer::HandleErrors(const string& fileName) const
{
#if HAVE_IL_IL_H
    bool ret = false;
    ILenum error;

    // check if we have any errors and log them accordingly
    while ((error = ilGetError()) != IL_NO_ERROR)
    {
        ret = true;

        string msg;

        switch(error)
        {
        case IL_INVALID_ENUM :
            msg = "invalid enum";
            break;

        case IL_OUT_OF_MEMORY :
            msg = "out of memory";
            break;

        case IL_FORMAT_NOT_SUPPORTED :
            msg = "format not supported";
            break;

        case IL_INTERNAL_ERROR :
            msg = "internal error";
            break;

        case IL_INVALID_VALUE :
            msg = "invalid value";
            break;

        case IL_ILLEGAL_OPERATION :
            msg = "illegal operation";
            break;

        case IL_ILLEGAL_FILE_VALUE :
            msg  = "illegal file value";
            break;

        case IL_INVALID_FILE_HEADER :
            msg = "invalid file header";
            break;

        case IL_INVALID_PARAM :
            msg  = "invalid param";
            break;

        case IL_COULD_NOT_OPEN_FILE :
            msg = "could not open file";
            break;

        case IL_INVALID_EXTENSION :
            msg = "invalid extension";
            break;

        case IL_FILE_ALREADY_EXISTS :
            msg = "file already exists";
            break;

        case IL_OUT_FORMAT_SAME :
            msg = "out format same";
            break;

        case IL_STACK_OVERFLOW :
            msg  ="stack overflow";
            break;

        case IL_STACK_UNDERFLOW :
            msg  ="stack underflow";
            break;

        case IL_INVALID_CONVERSION :
            msg = "invalid conversion";
            break;

        case IL_BAD_DIMENSIONS :
            msg = "bad dimensions";
            break;

        case IL_FILE_READ_ERROR :
            //case IL_FILE_WRITE_ERROR :
            msg = "file read/write error";
            break;

        case IL_LIB_GIF_ERROR :
            msg = "lib gif error";
            break;

        case IL_LIB_JPEG_ERROR :
            msg = "lib jpeg error";
            break;

        case IL_LIB_PNG_ERROR :
            msg = "lib png error";
            break;

        case IL_LIB_TIFF_ERROR :
            msg = "lib tiff error";
            break;

        case IL_LIB_MNG_ERROR :
            msg = "lib mng error";
            break;

        default:
            msg = "unknown IL error";
            break;
        }

        if (! fileName.empty())
            {
                msg = fileName + ": " + msg;
            }

        GetLog()->Error() << "(ImageServer) ERROR: DevIL returned error "
                          << error << " (" << msg << ")\n";
    }

    return ret;
#else
    return true;
#endif
}
