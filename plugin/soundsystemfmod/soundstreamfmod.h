#ifndef SOUNDSTREAMFMOD_H__
#define SOUNDSTREAMFMOD_H__

#include <kerosin/soundserver/soundstream.h>
#include <fmod/fmod.h>

class SoundStreamFMOD : public kerosin::SoundStream
{
public:
	SoundStreamFMOD(kerosin::SoundServer &inServer);
	virtual ~SoundStreamFMOD();

	void Load(const char *inName);
	void Load(void *inBuffer, int inSize);
	void Play();

protected:
	FSOUND_STREAM*	mHandle;
	char*			mBuffer;
};
#endif //SOUNDSTREAMFMOD_H__
