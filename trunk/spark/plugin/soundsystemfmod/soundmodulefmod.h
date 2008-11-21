/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soundmodulefmod.h,v 1.3 2004/04/10 11:59:46 fruit Exp $

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
#ifndef SOUNDMODULEFMOD_H__
#define SOUNDMODULEFMOD_H__

#include <kerosin/soundserver/soundmodule.h>
#include <fmod/fmod.h>

class SoundModuleFMOD : public kerosin::SoundModule
{
public:
    SoundModuleFMOD(kerosin::SoundServer &inServer);
    virtual ~SoundModuleFMOD();

    void Load(const std::string& inName);
    void Load(void *inBuffer, int inSize);
    void Play();

protected:
    FMUSIC_MODULE* mHandle;
};
#endif //SOUNDMODULEFMOD_H__
