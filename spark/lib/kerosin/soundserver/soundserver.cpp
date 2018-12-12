/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#include "soundserver.h"
#include "soundsystem.h"
#include "soundeffect.h"
#include "soundstream.h"
#include "soundmodule.h"
#include <salt/fileclasses.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/core.h>
#ifndef Q_MOC_RUN
#include <boost/scoped_ptr.hpp>
#endif

//------------------------------------------------------------------------------------------------
// SoundServer implementation
//------------------------------------------------------------------------------------------------

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace std;
using namespace zeitgeist;

// constructor
SoundServer::SoundServer() : Leaf(), mQuality(SOUNDQUALITY_BEST)
{
}

SoundServer::~SoundServer()
{
        Reset();
}

bool SoundServer::Init(const std::string &sndSysName)
{
        GetLog()->Normal().Printf("SoundServer::Init -> '%s'\n", sndSysName.c_str());
        Reset();

        // create the soundsystem
        mSoundSystem = static_pointer_cast<SoundSystem>(GetCore()->New(sndSysName));

        if(!mSoundSystem)
        {
                // could not create SoundSystem
                GetLog()->Error().Printf("ERROR: Unable to create '%s'\n", sndSysName.c_str());
                return false;
        }

        // we have a soundsystem, so initialize it
        if(mSoundSystem->Init(mQuality) == false)
        {
                // something happened when we wanted to initialize the soundsystem
                GetLog()->Error().Printf("ERROR: Could not init '%s'\n", sndSysName.c_str());
                return false;
        }

        return true;
}

float SoundServer::GetCPU()
{
        return mSoundSystem->GetCPU();
}

boost::shared_ptr<SoundEffect> SoundServer::LoadEffect(const string& inName)
{
        boost::shared_ptr<SoundObject> soundObject;

        if (LoadSoundObject(inName, mEffects, soundObject) == false)
                return boost::shared_ptr<SoundEffect>();

        if (soundObject)
        {
                GetLog()->Debug() << "Found a cached sound" << endl;
                return static_pointer_cast<SoundEffect>(soundObject);
        }

        // we don't have the sound in the cache, so create it
        boost::shared_ptr<SoundEffect> effect(mSoundSystem->CreateEffect(*this));

        // now, we want to load the file from our fileserver
        boost::shared_ptr<FileServer>  fileServer = static_pointer_cast<FileServer>(GetCore()->Get("/sys/server/file"));
        boost::shared_ptr<salt::RFile> file = fileServer->Open(inName.c_str());

        if(file.get() == NULL)
        {
                GetLog()->Error() << "ERROR: Could not open file" << endl;
                // could not open file for some strange reason
                return boost::shared_ptr<SoundEffect>();
        }

        boost::shared_ptr<char> buffer(new char[file->Size()]);
        file->Read(buffer.get(), file->Size());

        effect->Load(buffer.get(), file->Size());
        effect->SetFileName(inName);

        // now, we have to add it to the cache
        mEffects[inName] = effect;

        return effect;
}

boost::shared_ptr<SoundStream> SoundServer::LoadStream(const string& inName)
{
        GetLog()->Debug() << "SoundServer::LoadStream " << inName << endl;
        boost::shared_ptr<SoundObject> soundObject;

        if (LoadSoundObject(inName, mStreams, soundObject) == false)
                return boost::shared_ptr<SoundStream>();

        if (soundObject)
        {
                GetLog()->Debug() << "Found a cached sound" << endl;
                return static_pointer_cast<SoundStream>(soundObject);
        }

        // we don't have the sound in the cache, so create it
        boost::shared_ptr<SoundStream> stream(mSoundSystem->CreateStream(*this));

        // now, we want to load the file from our fileserver
        boost::shared_ptr<FileServer>  fileServer = static_pointer_cast<FileServer>(GetCore()->Get("/sys/server/file"));
        boost::shared_ptr<salt::RFile> file = fileServer->Open(inName.c_str());

        if(file.get() == NULL)
        {
                GetLog()->Error() << "ERROR: Could not open file" << endl;
                // could not open file for some strange reason
                return boost::shared_ptr<SoundStream>();
        }

        char* buffer = new char[file->Size()];
        file->Read(buffer, file->Size());

        stream->Load(buffer, file->Size());
        stream->SetFileName(inName);

        // now, we have to add it to the cache
        mStreams[inName] = stream;

        return stream;
}

boost::shared_ptr<SoundModule> SoundServer::LoadModule(const string& inName)
{
        boost::shared_ptr<SoundObject> soundObject;

        if (LoadSoundObject(inName, mModules, soundObject) == false)
                return boost::shared_ptr<SoundModule>();

        if (soundObject)
        {
                GetLog()->Debug() << "Found a cached sound" << endl;
                return static_pointer_cast<SoundModule>(soundObject);
        }

        // we don't have the sound in the cache, so create it
        boost::shared_ptr<SoundModule> module(mSoundSystem->CreateModule(*this));

        // now, we want to load the file from our fileserver
        boost::shared_ptr<FileServer> fileServer = static_pointer_cast<FileServer>(GetCore()->Get("/sys/server/file"));
        boost::shared_ptr<salt::RFile> file = fileServer->Open(inName.c_str());

        if(file.get() == NULL)
        {
                GetLog()->Error() << "ERROR: Could not open file" << endl;
                // could not open file for some strange reason
                return boost::shared_ptr<SoundModule>();
        }

        boost::shared_ptr<char> buffer(new char[file->Size()]);
        file->Read(buffer.get(), file->Size());

        module->Load(buffer.get(), file->Size());
        module->SetFileName(inName);

        // now, we have to add it to the cache
        mModules[inName] = module;

        return module;
}

void SoundServer::Reset()
{
        mSoundSystem.reset();

        TSoundHashMap::const_iterator i;

        mEffects.clear();

        mModules.clear();

        mStreams.clear();
}

bool SoundServer::LoadSoundObject(const std::string& inName, const TSoundHashMap& map, boost::shared_ptr<SoundObject> &soundObject) const
{
        // if we have no sound system loaded, then we can't load a sound
        if (mSoundSystem.get() == NULL)
        {
                GetLog()->Error() << "ERROR: No SoundSystem loaded!" << endl;
                soundObject = boost::shared_ptr<SoundObject>();
                return false;
        }

        // we have a sound system, so let's check if the sound has been loaded already
        TSoundHashMap::const_iterator i = map.find(inName);

        if(i != map.end())
        {
                soundObject = (*i).second;
        }
        else
        {
                soundObject = boost::shared_ptr<SoundObject>();
        }

        return true;
}
