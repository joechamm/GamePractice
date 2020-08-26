#ifndef __AL_AUDIO_SYSTEM_HPP__
#define __AL_AUDIO_SYSTEM_HPP__

#include "Core/SystemLogic.hpp"
#include "Core/ETTasks.hpp"
#include "Core/ETPrimitives.hpp"
#include "MixGraph/MixGraph.hpp"
#include "Audio/ETSoundPlayManager.hpp"

struct ALCdevice;
struct ALCcontext;
class ALSoundSource;

class ALAudioSystem : public SystemLogic,
    public ETNode<ETSoundUpdateTask>,
    public ETNode<ETSoundPlayManager> {
public:

    ALAudioSystem();
    virtual ~ALAudioSystem();

    // SystemLogic
    bool init() override;
    void deinit() override;

    // ETSoundUpdateTask
    void ET_updateSound() override;

    // ETSoundPlayManager
    bool ET_play(SoundStream* soundStream) override;

private:

    bool initSoundContext();
    bool initAlSource();

private:

    ALCdevice* alcDevice;
    ALCcontext* alcContext;
    MixGraph mixGrap;
    std::unique_ptr<float[]> mixBuffer;
    std::unique_ptr<unsigned int[]> alBufferIds;
    unsigned int alSourceId;
    bool alSourcePlaying;
};

#endif /* __AL_AUDIO_SYSTEM_HPP__ */