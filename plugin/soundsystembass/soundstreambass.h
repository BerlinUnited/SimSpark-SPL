#ifndef SOUNDSTREAMBASS_H__
#define SOUNDSTREAMBASS_H__

#include <kerosin/soundserver/soundstream.h>
#include <bass/bass.h>

class SoundStreamBASS : public kerosin::SoundStream
{
public:
	SoundStreamBASS(kerosin::SoundServer &inServer);
	virtual ~SoundStreamBASS();

	void Load(const char *inName);
	void Load(void *inBuffer, int inSize);
	void Play();

protected:
	HSTREAM	mHandle;
	char	*mBuffer;
};
#endif //SOUNDSTREAMBASS_H__