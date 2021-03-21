#pragma once
#include "DecodeThread.h"
#include <mutex>
#include <queue>

class Resample;
class AudioPlay;
class AVCodecParameters;

class AudioThread : public DecodeThread
{
public:
    AudioThread();
    virtual ~AudioThread();

    bool open(AVCodecParameters *param);
    void close();
    void clear();
    void setPause(bool pause);
    void setVolumeValue(double num);
    bool playToPts(AVPacket* pkt, int64_t& seekpts);
    
    int64_t pts() const { return m_pts; }
protected:
    virtual void run();
private:
    std::mutex  m_mutex;
    bool m_start = false;
    bool m_pause = false;
    Resample    *m_resample = nullptr;
    AudioPlay   *m_play = nullptr;
    int64_t     m_pts = 0;      // 当前播放的pts
};

