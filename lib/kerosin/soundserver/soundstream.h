#ifndef SOUNDSTREAM_H__
#define SOUNDSTREAM_H__

#include "soundobject.h"

namespace kerosin
{

class SoundStream : public SoundObject
{
public:
	SoundStream(SoundServer &inServer) : SoundObject(inServer) {}
	virtual ~SoundStream()	{}

	virtual void Load(const char *inName) = 0;
	virtual void Load(void *inBuffer, int inSize) = 0;
	virtual void Play() = 0;
};

} //namespace kerosin

#endif //SOUNDSTREAM_H__
