#ifndef SOUNDSYSTEMBASS_H__
#define SOUNDSYSTEMBASS_H__

// get the interface we want to implement
#include <kerosin/soundserver/soundsystem.h>
#include <zeitgeist/class.h>

class SoundSystemBASS : public kerosin::SoundSystem
{
public:
        SoundSystemBASS();
        virtual ~SoundSystemBASS();

        bool                    Init(int inFreq);
        void                    Shutdown();

        float                   GetCPU();

        kerosin::SoundEffect*   CreateEffect(kerosin::SoundServer &soundServer);
        kerosin::SoundStream*   CreateStream(kerosin::SoundServer &soundServer);
        kerosin::SoundModule*   CreateModule(kerosin::SoundServer &soundServer);
};

DECLARE_CLASS(SoundSystemBASS);

#endif //SOUNDSYSTEMBASS_H__
