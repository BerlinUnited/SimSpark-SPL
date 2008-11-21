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
#include "soundstreambass.h"

SoundStreamBASS::SoundStreamBASS(kerosin::SoundServer &inServer) : SoundStream(inServer), mBuffer(NULL), mHandle(NULL)
{
}

SoundStreamBASS::~SoundStreamBASS()
{
        delete[] mBuffer;
        if(mHandle != 0)
        {
                BASS_StreamFree(mHandle);
        }
}

void SoundStreamBASS::Load(const char *inName)
{
        mHandle = BASS_StreamCreateFile(FALSE, (void*)inName, 0, 0, 0);

        SetFileName(inName);
}

void SoundStreamBASS::Load(void *inBuffer, int inSize)
{
        mBuffer = (char*)inBuffer;
        mHandle = BASS_StreamCreateFile(TRUE, inBuffer, 0, inSize, 0);
}


void SoundStreamBASS::Play()
{
        if(mHandle != 0)
        {
                BASS_StreamPlay(mHandle, FALSE, BASS_SAMPLE_LOOP);
        }
}
