#ifndef SOUNDMODULE_H__
#define SOUNDMODULE_H__

#include "soundobject.h"

namespace kerosin
{

class SoundModule : public SoundObject
{
public:
	SoundModule(SoundServer &inServer) : SoundObject(inServer) {}
	virtual ~SoundModule()	{}

	virtual void Load(const char *inName) = 0;
	virtual void Load(void *inBuffer, int inSize) = 0;
	virtual void Play() = 0;
};

} //namespace kerosin

#endif //SOUNDMODULE_H__
