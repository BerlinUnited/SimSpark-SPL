/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: fileserver_c.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "fileserver.h"

using namespace boost;
using namespace zeitgeist;
using namespace std;

FUNCTION(FileServer,exist)
{
    string inName;

    return
        (
         (in.GetSize() == 1) &&
         (in.GetValue(in.begin(),inName)) &&
         (obj->Exist(inName.c_str()))
         );
}

FUNCTION(FileServer,mount)
{
    string inFsName;
    string inPath;

    return
        (
         (in.GetSize() == 2) &&
         (in.GetValue(in[0],inFsName)) &&
         (in.GetValue(in[1],inPath)) &&
         (obj->Mount(inFsName.c_str(),inPath.c_str()))
         );
}

FUNCTION(FileServer,unmount)
{
    bool ret = false;

    switch (in.GetSize())
        {
        default:
            break;

        case 1:
            {
                string inPath;
                if (in.GetValue(in.begin(),inPath))
                    {
                        ret = obj->Unmount(inPath.c_str());
                    }

                break;
            }

        case 2:
            {
                string inClass;
                string inPath;

                if (
                    (in.GetValue(in[0],inClass)) &&
                    (in.GetValue(in[1],inPath))
                    )
                    {
                        ret = obj->Unmount(inClass.c_str(),inPath.c_str());
                    }

                break;
            }
        }

    return ret;
}

void CLASS(FileServer)::DefineClass()
{
        DEFINE_BASECLASS(zeitgeist/Node);
        DEFINE_FUNCTION(exist);
        DEFINE_FUNCTION(mount);
        DEFINE_FUNCTION(unmount);
}
