#ifndef SOUNDEFFECTFMOD_H__
#define SOUNDEFFECTFMOD_H__

#include <kerosin/soundserver/soundeffect.h>
#include <fmod/fmod.h>

class SoundEffectFMOD : public kerosin::SoundEffect
{
public:
	SoundEffectFMOD(kerosin::SoundServer &inServer);
	virtual ~SoundEffectFMOD();

	void Load(const char *inName);
	void Load(void *inBuffer, int inSize);
	void Play();

protected:
	FSOUND_SAMPLE* mHandle;
};
#endif //SOUNDEFFECTFMOD_H__
