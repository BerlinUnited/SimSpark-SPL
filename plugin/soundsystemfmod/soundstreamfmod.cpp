#include "soundstreamfmod.h"

SoundStreamFMOD::SoundStreamFMOD(kerosin::SoundServer &inServer) : SoundStream(inServer), mHandle(NULL), mBuffer(NULL)
{
}

SoundStreamFMOD::~SoundStreamFMOD()
{
	delete[] mBuffer;
	if(mHandle != 0)
	{
		FSOUND_Stream_Close(mHandle);
	}
}

void SoundStreamFMOD::Load(const char* inName)
{
	mHandle = FSOUND_Stream_OpenFile(inName, FSOUND_LOOP_NORMAL, 0);
	SetFileName(inName);
}

void SoundStreamFMOD::Load(void *inBuffer, int inSize)
{
	mBuffer = (char*)inBuffer;
	mHandle = FSOUND_Stream_OpenFile( mBuffer, FSOUND_LOADMEMORY, inSize);
}

void SoundStreamFMOD::Play()
{
	if(mHandle != 0)
	{
		FSOUND_Stream_Play(FSOUND_FREE, mHandle);
	}
}
