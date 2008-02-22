/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: rubysceneimporter_c.cpp,v 1.3 2008/02/22 16:48:18 hedayat Exp $

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

#include "rubysceneimporter.h"

FUNCTION(RubySceneImporter,enableSceneDictionary)
{
    bool enable;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], enable))
        )
        {
            return false;
        }

    obj->EnableSceneDictionary(enable);
    return true;
}

FUNCTION(RubySceneImporter,setUnlinkOnCompleteScenes)
{
    bool inUnlink;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inUnlink))
        )
        {
            return false;
        }

    obj->SetUnlinkOnCompleteScenes(inUnlink);
    return true;
}

void CLASS(RubySceneImporter)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/SceneImporter);
    DEFINE_FUNCTION(setUnlinkOnCompleteScenes);
    DEFINE_FUNCTION(enableSceneDictionary);
}
