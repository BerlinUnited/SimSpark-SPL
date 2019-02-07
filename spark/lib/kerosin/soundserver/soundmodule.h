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
#ifndef KEROSIN_SOUNDMODULE_H
#define KEROSIN_SOUNDMODULE_H

#include "soundobject.h"

namespace kerosin
{

class SoundModule : public SoundObject
{
public:
    SoundModule(SoundServer &inServer) : SoundObject(inServer) {}
    virtual ~SoundModule()      {}

    virtual void Load(const std::string& inName) = 0;
    virtual void Load(void *inBuffer, int inSize) = 0;
    virtual void Play() = 0;
};

} //namespace kerosin

#endif //KEROSIN_SOUNDMODULE_H
