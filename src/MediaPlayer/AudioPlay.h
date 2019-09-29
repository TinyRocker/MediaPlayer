#pragma once
#include <mutex>

class QAudioOutput;
class QIODevice;

class AudioPlay
{
public:
    AudioPlay();
    virtual ~AudioPlay();

    bool open(int sampleRate, int sampleSize, int channels);
    void close();
    void clear();

    bool writeData(const char *data, int datasize);
    int freeSpaceSize();

    // 返回缓冲中还没有播放的时间 ms
    int64_t noPlayMs();
    void setPause(bool pause);
    void setVolumeValue(double num);
private:
    std::mutex m_mutex;
    bool       m_pause = false;
    QAudioOutput *m_output = nullptr;
    QIODevice    *m_io = nullptr;
    int m_sampleRate = 44100;
    int m_sampleSize = 16;
    int m_channels = 2;
};

