#ifndef SOUNDSYSTEMFMOD_H__
#define SOUNDSYSTEMFMOD_H__

// get the interface we want to implement
#include <kerosin/soundserver/soundsystem.h>
#include <zeitgeist/class.h>

class SoundSystemFMOD : public kerosin::SoundSystem
{
public:
        SoundSystemFMOD();
        virtual ~SoundSystemFMOD();

        bool                    Init(int inFreq);
        void                    Shutdown();

        float                   GetCPU();

        kerosin::SoundEffect*   CreateEffect(kerosin::SoundServer &soundServer);
        kerosin::SoundStream*   CreateStream(kerosin::SoundServer &soundServer);
        kerosin::SoundModule*   CreateModule(kerosin::SoundServer &soundServer);
};

DECLARE_CLASS(SoundSystemFMOD);

#endif //SOUNDSYSTEMFMOD_H__
