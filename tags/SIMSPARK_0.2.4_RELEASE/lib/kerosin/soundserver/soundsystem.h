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
#ifndef KEROSIN_SOUNDSYSTEM_H
#define KEROSIN_SOUNDSYSTEM_H

#include <zeitgeist/leaf.h>

namespace kerosin
{

class SoundEffect;
class SoundStream;
class SoundModule;
class SoundServer;

/*      SoundSystem

        Here we define the interface which an actual SoundSystem will need to
        implement. Actual SoundSystems would be derived from this interface as
        ClassServer plugins. You would (in general) associate a SoundSystem with
        a SoundLibrary, such as FMOD or BASS.

        NOTE:

        HISTORY:
                18.09.01 - MK
                        - Initial version

        TODO:
                - much more functionality (volume control, 3D sounds)

        TOFIX:
*/
class SoundSystem : public zeitgeist::Leaf
{
public:
    SoundSystem() : zeitgeist::Leaf()   {}
    virtual ~SoundSystem()      {}
    virtual bool                        Init(int inFreq) = 0;
    virtual void                        Shutdown() = 0;

    virtual float                       GetCPU() = 0;

    virtual SoundEffect*        CreateEffect(SoundServer &soundServer) = 0;
    virtual SoundStream*        CreateStream(SoundServer &soundServer) = 0;
    virtual SoundModule*        CreateModule(SoundServer &soundServer) = 0;
};

} //namespace kerosin

#endif //KEROSIN_SOUNDSYSTEM_H
