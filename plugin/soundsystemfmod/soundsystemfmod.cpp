#include "soundsystemfmod.h"
#include "soundeffectfmod.h"
#include "soundstreamfmod.h"
#include "soundmodulefmod.h"
#include <fmod/fmod.h>

SoundSystemFMOD::SoundSystemFMOD()
{
}

SoundSystemFMOD::~SoundSystemFMOD()
{
	Shutdown();
}

bool SoundSystemFMOD::Init(int inFreq)
{
	// initialize FMOD
	if(FSOUND_Init(inFreq, 100, 0) != 0)
	{
		return true;
	}

	return false;
}

void SoundSystemFMOD::Shutdown()
{
	FSOUND_Close();
}

float SoundSystemFMOD::GetCPU()
{
	return FSOUND_GetCPUUsage();
}

kerosin::SoundEffect* SoundSystemFMOD::CreateEffect(kerosin::SoundServer &soundServer)
{
	return new SoundEffectFMOD(soundServer);
}

kerosin::SoundStream* SoundSystemFMOD::CreateStream(kerosin::SoundServer &soundServer)
{
	return new SoundStreamFMOD(soundServer);
}

kerosin::SoundModule* SoundSystemFMOD::CreateModule(kerosin::SoundServer &soundServer)
{
	return new SoundModuleFMOD(soundServer);
}
