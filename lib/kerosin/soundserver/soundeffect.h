#ifndef SOUNDEFFECT_H__
#define SOUNDEFFECT_H__

#include "soundobject.h"

namespace kerosin
{

class SoundEffect : public SoundObject
{
public:
	SoundEffect(SoundServer &inServer) : SoundObject(inServer) {}
	virtual ~SoundEffect()	{}

	virtual void Load(const char *inName) = 0;
	virtual void Load(void *inBuffer, int inSize) = 0;
	virtual void Play() = 0;
};

} //namespace kerosin

#endif //SOUNDEFFECT_H__
