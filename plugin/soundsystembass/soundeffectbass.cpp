#include "soundeffectbass.h"

SoundEffectBASS::SoundEffectBASS(kerosin::SoundServer &inServer) : SoundEffect(inServer)
{
	mHandle = 0;
}

SoundEffectBASS::~SoundEffectBASS()
{
	if(mHandle != 0)
	{
		BASS_SampleFree(mHandle);
	}
}

void SoundEffectBASS::Load(const char* inName)
{
	mHandle = BASS_SampleLoad(FALSE, (void*)inName, 0, 0, 65535, 0);

	SetFileName(inName);
}

void SoundEffectBASS::Load(void *inBuffer, int inSize)
{
	mHandle = BASS_SampleLoad(TRUE, inBuffer, 0, inSize, 65535, 0);
}


void SoundEffectBASS::Play()
{
	if(mHandle != 0)
	{
		BASS_SamplePlay(mHandle);
	}
}
