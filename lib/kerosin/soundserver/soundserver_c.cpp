/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soundserver_c.cpp,v 1.3 2004/03/22 11:18:03 rollmark Exp $

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
#include "soundserver.h"
#include "soundeffect.h"
#include "soundmodule.h"
#include "soundstream.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;
using namespace std;

FUNCTION(SoundServer,init)
{
    string inSndSysName;

    return(
           (in.GetSize() == 1) &&
           (in.GetValue(in.begin(), inSndSysName)) &&
           (obj->Init(inSndSysName))
           );
}

FUNCTION(SoundServer,getCPU)
{
    return obj->GetCPU();
}

FUNCTION(SoundServer,playStream)
{
    string inName;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inName))
        )
        {
            return false;
        }

    shared_ptr<SoundStream> stream = obj->LoadStream(inName);

    if (stream.get() == 0)
        {
            return false;
        }

    stream->Play();
    return true;
}

FUNCTION(SoundServer,playModule)
{
    string inName;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inName))
        )
        {
            return false;
        }

    shared_ptr<SoundModule> module = obj->LoadModule(inName);

    if (module.get() == 0)
        {
            return false;
        }

    module->Play();
    return true;
}

FUNCTION(SoundServer,playEffect)
{
    string inName;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inName))
        )
        {
            return false;
        }

    shared_ptr<SoundEffect> effect = obj->LoadEffect(inName);

    if (effect.get() == 0)
        {
            return false;
        }

    effect->Play();
    return true;
}

void CLASS(SoundServer)::DefineClass()
{
        DEFINE_BASECLASS(zeitgeist/Leaf);
        DEFINE_FUNCTION(init);
        DEFINE_FUNCTION(getCPU);
        DEFINE_FUNCTION(playStream);
        DEFINE_FUNCTION(playModule);
        DEFINE_FUNCTION(playEffect);
}
