#ifndef SOUNDOBJECT_H__
#define SOUNDOBJECT_H__

#include <string>

namespace kerosin
{

class SoundServer;

class SoundObject
{
public:
	SoundObject(SoundServer &inServer) : mServer(inServer), mFileName("<null>") {}
	virtual ~SoundObject()	{}

	void		SetFileName(const char *inName)	{	mFileName = inName;	}

	const char*	GetFileName() const	{	return mFileName.c_str();	}

protected:
	SoundServer&	mServer;	// this is a reference to the server the sound belongs to
private:
	std::string		mFileName;	// the name of the file this sound came from
};

} //namespace kerosin

#endif //SOUNDOBJECT_H__
