#ifndef PERCEPTOR_H__
#define PERCEPTOR_H__

#include "../sceneserver/basenode.h"

#ifdef HAVE_HASH_MAP
#include <hash_map>
#else
#include <map>
#endif

namespace kerosin
{

class Perceptor : public kerosin::BaseNode
{
        //
        // types
        //
public:
#ifdef HAVE_HASH_MAP
        typedef std::hash_map<std::string, boost::any> TDictionary;
#else
        typedef std::map<std::string, boost::any> TDictionary;
#endif


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
