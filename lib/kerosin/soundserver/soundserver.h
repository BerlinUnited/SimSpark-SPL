#ifndef SOUNDSERVER_H__
#define SOUNDSERVER_H__

/*      $Id: soundserver.h,v 1.4 2003/08/28 20:00:37 rollmark Exp $

        SoundServer

        The SoundServer is the engine object which lets the application satisfy
        its audio needs. The SoundServer is an access layer to a specific SoundSystem.
        Another purpose of the SoundServer is managing a cache of SoundObjects. Actually,
        we use three distinctive caches, one for effects, one for modules and one for
        streams.

        NOTE:

        HISTORY:
                18.09.01 - MK
                        - Initial version
                20.09.01 - MK
                        - Added caching
                11.10.01 - MK
                        - Made singleton functionality more secure

        TODO:
                - expose more functionality

        TOFIX:
*/

#include <list>
#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

namespace kerosin
{

// forward declarations
class SoundSystem;
class SoundObject;
class SoundEffect;
class SoundStream;
class SoundModule;
class SystemWindow;

class SoundServer : public zeitgeist::Leaf
{
        //
        // Types
        //
public:
        // sound quality levels
        enum ESoundQuality
        {
                SOUNDQUALITY_BEST               = 48000,                // above CD quality             (slowest)
                SOUNDQUALITY_GOOD               = 44100,                // CD quality
                SOUNDQUALITY_AVERAGE    = 22000,                // radio quality
                SOUNDQUALITY_BAD                = 11000,                // bad quality
                SOUNDQUALITY_VERYBAD    =  8000                 // very bad quality             (fastest)
        };
private:
#ifdef HAVE_HASH_MAP
        typedef std::hash_map<std::string, boost::shared_ptr<SoundObject> >     TSoundHashMap;
#else
        typedef std::map<std::string, boost::shared_ptr<SoundObject> >  TSoundHashMap;
#endif

        //
        // Methods
        //
public:
        SoundServer();
        virtual ~SoundServer();

        bool                            Init(const std::string &sndSysName);

        float                           GetCPU();

        boost::shared_ptr<SoundEffect>  LoadEffect(const char *inName);
        boost::shared_ptr<SoundStream>  LoadStream(const char *inName);
        boost::shared_ptr<SoundModule>  LoadModule(const char *inName);

        //
        // Members
        //
private:
        //! this function resets the cached sounds in the hashmaps
        void    Reset();

        //! a helper function which wraps some common loading code (trivial rejects, etc..)
        bool    LoadSoundObject(const char *inName, const TSoundHashMap& map, boost::shared_ptr<SoundObject> &soundObject) const;

        boost::shared_ptr<SoundSystem>  mSoundSystem;

        TSoundHashMap   mEffects;
        TSoundHashMap   mModules;
        TSoundHashMap   mStreams;


        ESoundQuality   mQuality;       // the frequency which will be used     [default=SOUNDQUALITY_BEST]

        // make singleton functionality more secure
        SoundServer(const SoundServer&);
        SoundServer& operator=(const SoundServer&);
};

DECLARE_CLASS(SoundServer);

} //namespace kerosin

#endif //SOUNDSERVER_H__
