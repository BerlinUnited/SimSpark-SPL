#include "soundmodulebass.h"

SoundModuleBASS::SoundModuleBASS(kerosin::SoundServer &inServer) : SoundModule(inServer)
{
	mHandle = 0;
}

SoundModuleBASS::~SoundModuleBASS()
{
	if(mHandle != 0)
	{
		BASS_MusicFree(mHandle);
	}
}

void SoundModuleBASS::Load(const char *inName)
{
	mHandle = BASS_MusicLoad(FALSE, (void*)inName, 0, 0, 0);

	SetFileName(inName);
}

void SoundModuleBASS::Load(void *inBuffer, int inSize)
{
	mHandle = BASS_MusicLoad(TRUE, inBuffer, 0, inSize, 0);
}

void SoundModuleBASS::Play()
{
	if(mHandle != 0)
	{
		BASS_MusicPlay(mHandle);
	}
}
