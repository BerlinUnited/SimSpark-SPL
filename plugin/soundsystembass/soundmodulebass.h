#ifndef SOUNDMODULEBASS_H__
#define SOUNDMODULEBASS_H__

#include <kerosin/soundserver/soundmodule.h>
#include <bass/bass.h>

class SoundModuleBASS : public kerosin::SoundModule
{
public:
	SoundModuleBASS(kerosin::SoundServer &inServer);
	virtual ~SoundModuleBASS();

	void Load(const char *inName);
	void Load(void *inBuffer, int inSize);
	void Play();

protected:
	HMUSIC	mHandle;
};
#endif //SOUNDMODULEBASS_H__