/*	$Id: soundsystem.h,v 1.2 2003/04/30 14:21:46 fruit Exp $

	SoundSystem

	Here we define the interface which an actual SoundSystem will need to
	implement. Actual SoundSystems would be derived from this interface as
	ClassServer plugins. You would (in general) associate a SoundSystem with
	a SoundLibrary, such as FMOD or BASS.

	NOTE:

	HISTORY:
		18.09.01 - MK
			- Initial version

	TODO:
		- much more functionality (volume control, 3D sounds)

	TOFIX:
*/
#ifndef SOUNDSYSTEM_H__
#define SOUNDSYSTEM_H__

#include <zeitgeist/leaf.h>

namespace kerosin
{

class SoundEffect;
class SoundStream;
class SoundModule;
class SoundServer;

class SoundSystem : public zeitgeist::Leaf
{
public:
	SoundSystem() : zeitgeist::Leaf()	{}
	virtual ~SoundSystem()	{}
	virtual bool			Init(int inFreq) = 0;
	virtual void			Shutdown() = 0;

	virtual float			GetCPU() = 0;

	virtual SoundEffect*	CreateEffect(SoundServer &soundServer) = 0;
	virtual SoundStream*	CreateStream(SoundServer &soundServer) = 0;
	virtual SoundModule*	CreateModule(SoundServer &soundServer) = 0;
};

} //namespace kerosin

#endif //SOUNDSYSTEM_H__
