#ifndef SOUNDMODULEFMOD_H__
#define SOUNDMODULEFMOD_H__

#include <kerosin/soundserver/soundmodule.h>
#include <fmod/fmod.h>

class SoundModuleFMOD : public kerosin::SoundModule
{
public:
	SoundModuleFMOD(kerosin::SoundServer &inServer);
	virtual ~SoundModuleFMOD();

	void Load(const char *inName);
	void Load(void *inBuffer, int inSize);
	void Play();

protected:
	FMUSIC_MODULE* mHandle;
};
#endif //SOUNDMODULEFMOD_H__
