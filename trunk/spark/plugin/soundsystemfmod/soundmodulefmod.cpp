/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soundmodulefmod.cpp,v 1.3 2004/04/10 12:00:12 fruit Exp $

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
#include "soundmodulefmod.h"


SoundModuleFMOD::SoundModuleFMOD(kerosin::SoundServer& inServer) :
    SoundModule(inServer)
{
    mHandle = NULL;
}


SoundModuleFMOD::~SoundModuleFMOD()
{
    if (mHandle != NULL)
    {
        FMUSIC_FreeSong(mHandle);
    }
}


void SoundModuleFMOD::Load(const std::string& inName)
{
    mHandle = FMUSIC_LoadSong(inName.c_str());
    SetFileName(inName);
}


void SoundModuleFMOD::Load(void *inBuffer, int inSize)
{
    // has been changed to FMUSIC_LoadSongEx
    // mHandle = FMUSIC_LoadSongMemory(inBuffer, inSize);
    mHandle = FMUSIC_LoadSongEx((const char*) inBuffer, 0, inSize, FSOUND_LOADMEMORY, 0, 0);
}


void SoundModuleFMOD::Play()
{
    FMUSIC_PlaySong(mHandle);
}
