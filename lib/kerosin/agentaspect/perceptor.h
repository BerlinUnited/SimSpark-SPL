#ifndef PERCEPTOR_H__
#define PERCEPTOR_H__

#include "../sceneserver/basenode.h"

namespace kerosin
{

class Perceptor : public kerosin::BaseNode
{
	//
	// types
	//
public:
	typedef std::hash_map<std::string, boost::any> TDictionary;

	//
	// functions
	//
public:
	/*!
		This is called by agents to trigger the percept event implemented by
		this perceptor. The perceptor can return data through the dictionary,
		which is passed as a parameter. Returns true, if valid data is
		available and false otherwise.
	*/
	virtual bool Percept(TDictionary &dictionary) = 0;
};

DECLARE_ABSTRACTCLASS(Perceptor);

} // namespace kerosin

#endif //PERCEPTOR_H__
