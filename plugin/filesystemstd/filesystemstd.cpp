/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: filesystemstd.cpp,v 1.3 2004/02/12 14:07:24 fruit Exp $

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
#include "filesystemstd.h"
#include <salt/fileclasses.h>

using namespace salt;

// constructor
FileSystemSTD::FileSystemSTD()
{
}

// destructor
FileSystemSTD::~FileSystemSTD()
{
}

bool FileSystemSTD::SetPath(const char* inPath)
{
        // release memory for old path
        mPath = inPath;

        return true;
}

//
// This function is really simple. It appends inName to mPath and
// tries to open the combined name as a readonly file.
//
salt::RFile* FileSystemSTD::Open(const char* inName)
{
        //Assert(inName != NULL, "No name specified for openening a file");

        salt::RFile*    file            = new StdFile();
        std::string     fileName        = mPath;

        fileName += inName;     // append inName

        if(file->Open(fileName.c_str()) == false)
        {
                // hmm, couldn't open file
                delete file;
                return NULL;
        }

        return file;
}

int FileSystemSTD::ForEachFile(const char* expression, TCallback callback, void* param)
{
        // todo
        return 0;
}
