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
#ifndef KEROSIN_SOUNDSERVER_H
#define KEROSIN_SOUNDSERVER_H

/*      $Id$

        SoundServer

        The SoundServer is the engine object which lets the application satisfy
        its audio needs. The SoundServer is an access layer to a specific SoundSystem.
        Another purpose of the SoundServer is managing a cache of SoundObjects. Actually,
        we use three distinctive caches, one for effects, one for modules and one for
        streams.

        NOTE:

        HISTORY:
                18.09.01 - MK
                        - Initial version
                20.09.01 - MK
                        - Added caching
                11.10.01 - MK
                        - Made singleton functionality more secure

        TODO:
                - expose more functionality

        TOFIX:
*/

#include <list>
#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <kerosin/kerosin_defines.h>

namespace kerosin
{

// forward declarations
class SoundSystem;
class SoundObject;
class SoundEffect;
class SoundStream;
class SoundModule;
class SystemWindow;

class KEROSIN_API SoundServer : public zeitgeist::Leaf
{
    //
    // Types
    //
public:
    // sound quality levels
    enum ESoundQuality
    {
        SOUNDQUALITY_BEST               = 48000,                // above CD quality             (slowest)
        SOUNDQUALITY_GOOD               = 44100,                // CD quality
        SOUNDQUALITY_AVERAGE    = 22000,                // radio quality
        SOUNDQUALITY_BAD                = 11000,                // bad quality
        SOUNDQUALITY_VERYBAD    =  8000                 // very bad quality             (fastest)
    };
private:
#ifdef HAVE_HASH_MAP
    typedef std::hash_map<std::string, boost::shared_ptr<SoundObject> >     TSoundHashMap;
#else
    typedef std::map<std::string, boost::shared_ptr<SoundObject> >  TSoundHashMap;
#endif

    //
    // Methods
    //
public:
    SoundServer();
    virtual ~SoundServer();

    bool                            Init(const std::string &sndSysName);

    float                           GetCPU();

    boost::shared_ptr<SoundEffect>  LoadEffect(const std::string& inName);
    boost::shared_ptr<SoundStream>  LoadStream(const std::string& inName);
    boost::shared_ptr<SoundModule>  LoadModule(const std::string& inName);

    //
    // Members
    //
private:
    //! this function resets the cached sounds in the hashmaps
    void    Reset();

    //! a helper function which wraps some common loading code (trivial rejects, etc..)
    bool    LoadSoundObject(const std::string& inName, const TSoundHashMap& map, boost::shared_ptr<SoundObject> &soundObject) const;

    boost::shared_ptr<SoundSystem>  mSoundSystem;

    TSoundHashMap   mEffects;
    TSoundHashMap   mModules;
    TSoundHashMap   mStreams;


    ESoundQuality   mQuality;       // the frequency which will be used     [default=SOUNDQUALITY_BEST]

    // make singleton functionality more secure
    SoundServer(const SoundServer&);
    SoundServer& operator=(const SoundServer&);
};

DECLARE_CLASS(SoundServer);

} //namespace kerosin

#endif //KEROSIN_SOUNDSERVER_H
