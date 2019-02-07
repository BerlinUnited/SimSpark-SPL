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
#include "soundstreamfmod.h"

SoundStreamFMOD::SoundStreamFMOD(kerosin::SoundServer &inServer) :
    SoundStream(inServer), mHandle(NULL), mBuffer(NULL)
{
}

SoundStreamFMOD::~SoundStreamFMOD()
{
    delete[] mBuffer;
    if(mHandle != 0)
    {
        FSOUND_Stream_Close(mHandle);
    }
}

void SoundStreamFMOD::Load(const std::string& inName)
{
    mHandle = FSOUND_Stream_Open(inName.c_str(), FSOUND_LOOP_NORMAL, 0, 0);
    SetFileName(inName);
}

void SoundStreamFMOD::Load(void *inBuffer, int inSize)
{
    mBuffer = (char*)inBuffer;
    mHandle = FSOUND_Stream_Open((const char *) mBuffer, FSOUND_LOADMEMORY, 0, inSize);
}

void SoundStreamFMOD::Play()
{
    if(mHandle != 0)
    {
        FSOUND_Stream_Play(FSOUND_FREE, mHandle);
    }
}
