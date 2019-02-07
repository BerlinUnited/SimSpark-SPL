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
#ifndef KEROSIN_SOUNDOBJECT_H
#define KEROSIN_SOUNDOBJECT_H

#include <string>

namespace kerosin
{

class SoundServer;

class SoundObject
{
public:
    SoundObject(SoundServer &inServer) : mServer(inServer), mFileName("<null>") {}
    virtual ~SoundObject()      {}

    void SetFileName(const std::string& inName)
    {       mFileName = inName;     }

    const std::string&  GetFileName() const
    {       return mFileName;       }

protected:
    // this is a reference to the server the sound belongs to
    SoundServer& mServer;
private:
    // the name of the file this sound came from
    std::string mFileName;
};

} //namespace kerosin

#endif //KEROSIN_SOUNDOBJECT_H
