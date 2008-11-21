/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soundsystembass.cpp,v 1.2 2003/12/21 23:36:39 fruit Exp $

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
#include "soundsystembass.h"
#include "soundeffectbass.h"
#include "soundstreambass.h"
#include "soundmodulebass.h"
#include <bass/bass.h>

SoundSystemBASS::SoundSystemBASS()
{
}

float SoundSystemBASS::GetCPU()
{
        return BASS_GetCPU();
}

SoundSystemBASS::~SoundSystemBASS()
{
        Shutdown();
}

bool SoundSystemBASS::Init(int inFreq)
{
        // initialize BASS
        if(BASS_Init(-1, inFreq, 0, 0) == TRUE)
        {
                BASS_Start();
                return true;
        }

        return false;
}

void SoundSystemBASS::Shutdown()
{
        BASS_Free();
}

kerosin::SoundEffect* SoundSystemBASS::CreateEffect(kerosin::SoundServer &soundServer)
{
        return new SoundEffectBASS(soundServer);
}

kerosin::SoundStream* SoundSystemBASS::CreateStream(kerosin::SoundServer &soundServer)
{
        return new SoundStreamBASS(soundServer);
}

kerosin::SoundModule* SoundSystemBASS::CreateModule(kerosin::SoundServer &soundServer)
{
        return new SoundModuleBASS(soundServer);
}
