#include "soundmodulefmod.h"


SoundModuleFMOD::SoundModuleFMOD(kerosin::SoundServer &inServer) : SoundModule(inServer)
{
	mHandle = NULL;
}


SoundModuleFMOD::~SoundModuleFMOD()
{
	if (mHandle != NULL)
	{
		FMUSIC_FreeSong(mHandle);
	}
}


void SoundModuleFMOD::Load(const char *inName)
{
	mHandle = FMUSIC_LoadSong(inName);
	SetFileName(inName);
}


void SoundModuleFMOD::Load(void *inBuffer, int inSize)
{
	mHandle = FMUSIC_LoadSongMemory(inBuffer, inSize);
}


void SoundModuleFMOD::Play()
{
	FMUSIC_PlaySong(mHandle);
}
