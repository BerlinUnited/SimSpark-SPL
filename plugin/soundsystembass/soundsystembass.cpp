#include "soundsystembass.h"
#include "soundeffectbass.h"
#include "soundstreambass.h"
#include "soundmodulebass.h"
#include <bass/bass.h>

SoundSystemBASS::SoundSystemBASS()
{
}

float SoundSystemBASS::GetCPU()
{
	return BASS_GetCPU();
}

SoundSystemBASS::~SoundSystemBASS()
{
	Shutdown();
}

bool SoundSystemBASS::Init(int inFreq)
{
	// initialize BASS
	if(BASS_Init(-1, inFreq, 0, 0) == TRUE)
	{
		BASS_Start();
		return true;
	}

	return false;
}

void SoundSystemBASS::Shutdown()
{
	BASS_Free();
}

kerosin::SoundEffect* SoundSystemBASS::CreateEffect(kerosin::SoundServer &soundServer)
{
	return new SoundEffectBASS(soundServer);
}

kerosin::SoundStream* SoundSystemBASS::CreateStream(kerosin::SoundServer &soundServer)
{
	return new SoundStreamBASS(soundServer);
}

kerosin::SoundModule* SoundSystemBASS::CreateModule(kerosin::SoundServer &soundServer)
{
	return new SoundModuleBASS(soundServer);
}