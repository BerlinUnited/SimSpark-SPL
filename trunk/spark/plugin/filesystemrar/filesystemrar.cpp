/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: filesystemrar.cpp,v 1.5 2008/02/24 10:18:09 rollmark Exp $

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
#include "filesystemrar.h"
#include <salt/fileclasses.h>
#include <windows.h>
#include <unrar/unrar.h>

using namespace salt;
using namespace std;
using namespace boost;

static int index = 0;

static int PASCAL RARCallback(UINT msg, LONG UserData, LONG P1, LONG P2)
{
    unsigned char *buffer = (unsigned char*)UserData;

    switch(msg)
        {
        case UCM_PROCESSDATA:
            memcpy(&buffer[index], (const void*)P1, P2);
            index += P2;
            return 1;
        }

    return 0;
}

// constructor
FileSystemRAR::FileSystemRAR()
    : FileSystem()
{
}

// destructor
FileSystemRAR::~FileSystemRAR()
{
}

bool FileSystemRAR::SetPath(const string& inPath)
{
    mArchiveName = inPath;
    FILE *f = fopen(inPath, "rb");
    bool ret = (f != NULL);

    if (f) fclose(f);

    return ret;
}

//
// This function is really simple. It appends inName to mPath and
// tries to open the combined name as a readonly file.
//
shared_ptr<salt::RFile> FileSystemRAR::Open(const string& inName)
{
    RAROpenArchiveData      openArchiveData;

    memset(&openArchiveData, 0, sizeof(RAROpenArchiveData));
    openArchiveData.ArcName         = (char*)mArchiveName.c_str();
    openArchiveData.OpenMode        = RAR_OM_EXTRACT;

    HANDLE handle = RAROpenArchive(&openArchiveData);

    if (openArchiveData.OpenResult)
        {
            RARCloseArchive(handle);
            // something went wrong
            return shared_ptr<salt::RFile>();
        }

    // set the callback
    RARSetCallback(handle, NULL, 0);

    RARHeaderData headerData;

    while (RARReadHeader(handle, &headerData)==0)
        {
            if (strcmp(headerData.FileName, inName) == 0)
                {
                    // we have a match
                    unsigned char *buffer = new unsigned char[headerData.UnpSize];
                    shared_ptr<MemFile> file(new MemFile());
                    file->Open(buffer, headerData.UnpSize);
                    RARSetCallback(handle, RARCallback, (LONG)buffer);
                    index = 0;
                    if (RARProcessFile(handle, RAR_TEST, NULL, NULL) != 0)
                        {
                            file.reset();
                        }
                    RARSetCallback(handle, NULL, 0);
                    RARCloseArchive(handle);

                    return shared_dynamic_cast<salt::RFile>(file);
                }
            else
                {
                    RARProcessFile(handle, RAR_SKIP, NULL, NULL);
                }
        }

    RARSetCallback(handle, NULL, 0);
    RARCloseArchive(handle);

    return shared_ptr<salt::RFile>();
}

int FileSystemRAR::ForEachFile(const string& expression, TCallback callback,
                               void* param)
{
    // todo
    return 0;
}
