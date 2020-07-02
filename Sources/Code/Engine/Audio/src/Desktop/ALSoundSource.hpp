#ifndef __AL_SOUND_SOURCE_HPP__
#define __AL_SOUND_SOURCE_HPP__

#include "SoundSource.hpp"

#include <vector>

const int MAX_BUFFERS_PER_STREAM = 4;

class ALSoundSource : public SoundSource {
public:

    ALSoundSource(unsigned int newSourceId);
    virtual ~ALSoundSource();

    bool init();
    void update();

    // SoundSource
    void attachToDataStream(OggDataStream& newDataStream) override;
    void stopStreaming() override;
    void pauseStreaming() override;
    void resumeStreaming() override;
    void setGain(float newGain) override;
    void setLoop(bool loopFlag) override;
    bool isLooped() const override;
    bool isPaused() const override;

private:

    bool isStreaming() const;

private:

    enum class EBufferFillRes {
        Normal = 0,
        EndOfStream
    };

    enum class ESourceState {
        Normal = 0,
        WaitEnd,
        Ended
    };

private:

    ALSoundSource(const ALSoundSource&) = delete;
    ALSoundSource& operator=(const ALSoundSource&) = delete;

    void queueALBuffers(unsigned int* bufferIds, int size);
    EBufferFillRes fillALBuffer(unsigned int bufferId);
    void startALSource();
    void resetALSourceParams();

private:

    unsigned int alBufferIds[MAX_BUFFERS_PER_STREAM];
    OggDataStream* dataStream;
    unsigned int sourceId;
    ESourceState state;
    bool looping;
};

#endif /* __AL_SOUND_SOURCE_HPP__ */