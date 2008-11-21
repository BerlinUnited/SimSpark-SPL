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
#include "soundeffectfmod.h"

SoundEffectFMOD::SoundEffectFMOD(kerosin::SoundServer& inServer) : SoundEffect(inServer)
{
    mHandle = NULL;
}

SoundEffectFMOD::~SoundEffectFMOD()
{
    if (mHandle != NULL)
    {
        FSOUND_Sample_Free(mHandle);
    }
}

void
SoundEffectFMOD::Load(const std::string& inName)
{
    // Loading the sound as a Loop off, 8bit mono, signed, not hardware accelerated sample
    mHandle = FSOUND_Sample_Load(FSOUND_FREE, inName.c_str(),
                                 FSOUND_NORMAL, 0, 0);
    SetFileName(inName);
}

void
SoundEffectFMOD::Load(void *inBuffer, int inSize)
{
    // Loading the sound as a Loop off, 8bit mono, signed, not hardware accelerated sample

    mHandle = FSOUND_Sample_Load(FSOUND_FREE, (const char*)inBuffer,
                                 (FSOUND_NORMAL | FSOUND_LOADMEMORY), inSize, 0);
}

void
SoundEffectFMOD::Play()
{
    if (mHandle != NULL)
    {
        FSOUND_PlaySound(FSOUND_FREE, mHandle);
    }
}
