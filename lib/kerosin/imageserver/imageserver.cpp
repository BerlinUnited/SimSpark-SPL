/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: imageserver.cpp,v 1.5 2004/04/08 06:58:35 rollmark Exp $

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
#include <boost/scoped_ptr.hpp>

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;

shared_ptr<FileServer> gFileServer;

//------------------------------------------------------------------------------------------------
// FileServer hooks for DevIL
//------------------------------------------------------------------------------------------------
ILHANDLE ILAPIENTRY FSOpen(const ILstring inName)
{
    return gFileServer->Open(inName);
    //return fopen(inName, "rb");
}

ILvoid ILAPIENTRY FSClose(ILHANDLE handle)
{
    //printf("CLOSE\n");
    salt::RFile *file = (salt::RFile*)handle;
    file->Close();
    delete file;
    //fclose((FILE*)handle);
}

ILboolean ILAPIENTRY FSEof(ILHANDLE handle)
{
    salt::RFile *file = (salt::RFile*)handle;

    const ILboolean val= file->Eof();
    //gFileServer->GetLog()->Debug().Printf("eof: %x\n", val);
    return val;
}

ILint ILAPIENTRY FSGetc(ILHANDLE handle)
{
    salt::RFile *file = (salt::RFile*)handle;

    const int val = file->Getc();
    //gFileServer->GetLog()->Debug().Printf("getc: %x\n", val);
    return val;
}

ILint ILAPIENTRY FSRead(void *buffer, ILuint size, ILuint count, ILHANDLE handle)
{
    salt::RFile *file = (salt::RFile*)handle;

    const size_t readSize = file->Read(buffer, size, count);
    //gFileServer->GetLog()->Debug().Printf("read(%d, %d): %d\n", size, count, readSize);
    return readSize;
}

ILint ILAPIENTRY FSSeek(ILHANDLE handle, ILint offset, ILint origin)
{
    salt::RFile *file = (salt::RFile*)handle;

    const int val = file->Seek(offset, origin);
    //gFileServer->GetLog()->Debug().Printf("seek: %d\n", val);
    return val;
}

ILint ILAPIENTRY FSTell(ILHANDLE handle)
{
    salt::RFile *file = (salt::RFile*)handle;

    const int val = file->Tell();
    //gFileServer->GetLog()->Debug().Printf("tell: %d\n", val);
    return val;
}

//------------------------------------------------------------------------------------------------
// ImageServer implementation
//------------------------------------------------------------------------------------------------

// constructor
ImageServer::ImageServer()
{
    // initialize DevIL
    ilInit();

    // and setup the default behavior
    // (this might come out of a config file at a later point)
    ilEnable(IL_FILE_OVERWRITE);
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

    // register FileServer hooks for DevIL
    ilSetRead(
              FSOpen,
              FSClose,
              FSEof,
              FSGetc,
              FSRead,
              FSSeek,
              FSTell);
}

//
// This function loads the file inName. If inType is IL_TYPE_UNKNOWN,
// then the library will try to find a handler by the file extension provided.
// This behavior is done automatically by the library!
//
boost::shared_ptr<Image> ImageServer::Load(const char *inName, ILenum inType)
{
    // create a new image
    boost::shared_ptr<Image> image(new Image());

    // make it active with DevIL
    image->Bind();

    // set the file server
    gFileServer = shared_static_cast<FileServer>(GetCore()->Get("/sys/server/file"));

    // load the image
    ilLoad(inType, (ILstring)inName);

    // set the file server to 0 again
    gFileServer.reset();

    // check for errors
    if(HandleErrors() == true)
        {
            // release the image and return
            return boost::shared_ptr<Image>();
        }

    return image;
}

bool ImageServer::Save(const boost::shared_ptr<Image> &inImage, const char *inName, ILenum inType)
{
    // make the image active
    inImage->Bind();

    // set the file server
    gFileServer = shared_static_cast<FileServer>(GetCore()->Get("/sys/server/file"));

    // save the image
    ilSave(inType, (ILstring)inName);

    // set the file server to 0 again
    gFileServer.reset();

    // check for errors
    if(HandleErrors() == true)
        {
            return false;
        }

    return true;
}

//
//      This routine checks for DevIL errors and logs them. The function returns
//      'true' if an error has occured and 'false' if not.
//
bool ImageServer::HandleErrors()
{
    bool ret = false;
    ILenum error;

    // check if we have any errors and log them accordingly
    while ((error = ilGetError()) != IL_NO_ERROR)
        {
            ret = true;
            GetLog()->Error().Printf("DevIL Error: %x\n", error);
        }

    return ret;
}
