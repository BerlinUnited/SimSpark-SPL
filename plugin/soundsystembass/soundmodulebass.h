/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soundmodulebass.h,v 1.2 2003/12/21 23:36:39 fruit Exp $

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
#ifndef SOUNDMODULEBASS_H__
#define SOUNDMODULEBASS_H__

#include <kerosin/soundserver/soundmodule.h>
#include <bass/bass.h>

class SoundModuleBASS : public kerosin::SoundModule
{
public:
        SoundModuleBASS(kerosin::SoundServer &inServer);
        virtual ~SoundModuleBASS();

        void Load(const char *inName);
        void Load(void *inBuffer, int inSize);
        void Play();

protected:
        HMUSIC  mHandle;
};
#endif //SOUNDMODULEBASS_H__
