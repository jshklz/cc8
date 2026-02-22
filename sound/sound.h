#ifndef SOUND_H
#define SOUND_H

#include <os/os.h>
#include <base/base.h>
#include <memory/arena.h>

#include <third-party/miniaudio/miniaudio.h>


typedef enum SND_SoundFlag SND_SoundFlag;
enum SND_SoundFlag
{
    SND_AudioPaused=0,
    SND_AudioPlaying=1,
    SND_AudioStopped=2
};

typedef struct SND_Sound SND_Sound;
struct SND_Sound
{
    u8* path;
    r32 duration;
    i32 sound_paused_at;
    
    
    ma_decoder decoder;
    ma_result result;
    ma_device device;
    SND_SoundFlag flag;
    
};

EPOXY_function SND_Sound* SND_CreateSound(u8* path);
EPOXY_function void SND_PlaySound(SND_Sound* sound);
EPOXY_function void SND_ResumeSound(SND_Sound* sound);

EPOXY_function void SND_SetSoundTime(SND_Sound* sound, i32 position);
EPOXY_function r32 SND_GetElapsedSoundTime(SND_Sound* sound);
EPOXY_function r32 SND_GetSoundVolume(SND_Sound* sound);
EPOXY_function void SND_SetSoundVolume(SND_Sound* sound, r32 volume);
EPOXY_function void SND_PauseSound(SND_Sound* sound);
EPOXY_function SND_SoundFlag SND_GetSoundState(SND_Sound* sound);
#endif

