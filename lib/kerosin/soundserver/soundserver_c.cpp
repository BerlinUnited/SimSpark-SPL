#include "soundserver.h"
#include "soundeffect.h"
#include "soundmodule.h"
#include "soundstream.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(init)
{
	if (in.size() == 1)
	{
		SoundServer *ss = static_cast<SoundServer*>(obj);
		ss->Init(boost::any_cast<char*>(in[0]));
	}
}

FUNCTION(playStream)
{
	if (in.size() == 1)
	{
		SoundServer *ss = static_cast<SoundServer*>(obj);
		shared_ptr<SoundStream> stream = ss->LoadStream(boost::any_cast<char*>(in[0]));
		if (stream) stream->Play();
	}
}

FUNCTION(playModule)
{
	if (in.size() == 1)
	{
		SoundServer *ss = static_cast<SoundServer*>(obj);
		shared_ptr<SoundModule> module = ss->LoadModule(boost::any_cast<char*>(in[0]));
		if (module) module->Play();
	}
}

FUNCTION(playEffect)
{
	if (in.size() == 1)
	{
		SoundServer *ss = static_cast<SoundServer*>(obj);
		shared_ptr<SoundEffect> effect = ss->LoadEffect(boost::any_cast<char*>(in[0]));
		if (effect) effect->Play();
	}
}

void CLASS(SoundServer)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Leaf);
	DEFINE_FUNCTION(init);
	DEFINE_FUNCTION(playStream);
	DEFINE_FUNCTION(playModule);
	DEFINE_FUNCTION(playEffect);
}
