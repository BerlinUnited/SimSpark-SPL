#include "soundstreambass.h"

SoundStreamBASS::SoundStreamBASS(kerosin::SoundServer &inServer) : SoundStream(inServer), mBuffer(NULL), mHandle(NULL)
{
}

SoundStreamBASS::~SoundStreamBASS()
{
	delete[] mBuffer;
	if(mHandle != 0)
	{
		BASS_StreamFree(mHandle);
	}
}

void SoundStreamBASS::Load(const char *inName)
{
	mHandle = BASS_StreamCreateFile(FALSE, (void*)inName, 0, 0, 0);

	SetFileName(inName);
}

void SoundStreamBASS::Load(void *inBuffer, int inSize)
{
	mBuffer = (char*)inBuffer;
	mHandle = BASS_StreamCreateFile(TRUE, inBuffer, 0, inSize, 0);
}


void SoundStreamBASS::Play()
{
	if(mHandle != 0)
	{
		BASS_StreamPlay(mHandle, FALSE, BASS_SAMPLE_LOOP);
	}
}
