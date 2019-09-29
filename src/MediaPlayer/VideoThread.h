#pragma once
#include "DecodeThread.h"
#include <mutex>

class AVCodecParameters;
class VideoPlayInterface;

class VideoThread : public DecodeThread
{
public:
    VideoThread();
    virtual ~VideoThread();

    bool open(AVCodecParameters *param, VideoPlayInterface *videoInter);
    void close();
    void clear();

    // 解码pts,若接收的解码数据的pts >= seekpts, return true并显示图像同时修改seekpts为当前frame的pts
    bool repaintPts(AVPacket *pkt, int64_t& seekpts);
    void setPause(bool pause);

    void syncPts(long long pts) { m_syncPts = pts; }
    VideoPlayInterface *videoPlayHandle() const { return m_play; }
protected:
    virtual void run();
private:
    std::mutex  m_mutex;
    bool m_start = false;
    bool m_pause = false;
    VideoPlayInterface *m_play = nullptr;
    long long m_syncPts = 0;    // 同步时间，由外部传入
};