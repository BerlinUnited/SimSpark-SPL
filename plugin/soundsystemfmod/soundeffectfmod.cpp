#include "soundeffectfmod.h"

SoundEffectFMOD::SoundEffectFMOD(kerosin::SoundServer &inServer) : SoundEffect(inServer)
{
        mHandle = NULL;
}

SoundEffectFMOD::~SoundEffectFMOD()
{
        if (mHandle != NULL)
        {
                FSOUND_Sample_Free(mHandle);
        }
}

void SoundEffectFMOD::Load(const char* inName)
{
        // Loading the sound as a Loop off, 8bit mono, signed, not hardware accelerated sample
        mHandle = FSOUND_Sample_Load(FSOUND_FREE, inName, FSOUND_NORMAL, 0, 0);
        SetFileName(inName);
}

void SoundEffectFMOD::Load(void *inBuffer, int inSize)
{
        // Loading the sound as a Loop off, 8bit mono, signed, not hardware accelerated sample

        mHandle = FSOUND_Sample_Load(FSOUND_FREE, (const char*)inBuffer, (FSOUND_NORMAL | FSOUND_LOADMEMORY), inSize, 0);
}

void SoundEffectFMOD::Play()
{
        if (mHandle != NULL)
        {
                FSOUND_PlaySound(FSOUND_FREE, mHandle);
        }
}
