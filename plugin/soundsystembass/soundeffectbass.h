#ifndef SOUNDEFFECTBASS_H__
#define SOUNDEFFECTBASS_H__

#include <kerosin/soundserver/soundeffect.h>
#include <bass/bass.h>

class SoundEffectBASS : public kerosin::SoundEffect
{
public:
	SoundEffectBASS(kerosin::SoundServer &inServer);
	virtual ~SoundEffectBASS();
	void Load(const char *inName);
	void Load(void *inBuffer, int inSize);
	void Play();

protected:
	HSAMPLE	mHandle;
};
#endif //SOUNDEFFECTBASS_H__
