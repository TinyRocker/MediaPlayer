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

    // ����pts,�����յĽ������ݵ�pts >= seekpts, return true����ʾͼ��ͬʱ�޸�seekptsΪ��ǰframe��pts
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
    long long m_syncPts = 0;    // ͬ��ʱ�䣬���ⲿ����
};