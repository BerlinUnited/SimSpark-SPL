/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soundstreamfmod.h,v 1.3 2004/04/10 12:00:41 fruit Exp $

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
#ifndef SOUNDSTREAMFMOD_H__
#define SOUNDSTREAMFMOD_H__

#include <kerosin/soundserver/soundstream.h>
#include <fmod/fmod.h>

class SoundStreamFMOD : public kerosin::SoundStream
{
public:
    SoundStreamFMOD(kerosin::SoundServer &inServer);
    virtual ~SoundStreamFMOD();

    void Load(const std::string& inName);
    void Load(void *inBuffer, int inSize);
    void Play();

protected:
    FSOUND_STREAM*  mHandle;
    char* mBuffer;
};

#endif //SOUNDSTREAMFMOD_H__
