#pragma once
#include "DecodeThread.h"
#include <mutex>
#include <QTimer>

class AVCodecParameters;
class VideoPlayInterface;
class AVFrame;

class VideoThread : public DecodeThread
{
    Q_OBJECT

public:
    VideoThread();
    virtual ~VideoThread();

    bool open(AVCodecParameters *param, VideoPlayInterface *videoInter);
    void close();
    void clear();

    // 解码pts,若接收的解码数据的pts >= seekpts, return true并显示图像同时修改seekpts为当前frame的pts
    bool repaintToPts(AVPacket *pkt, int64_t& seekpts);
    void setPause(bool pause);

    void syncPts(long long pts) { m_syncPts = pts; }
    VideoPlayInterface *videoPlayHandle() const { return m_play; }
    int64_t pts() { return m_pts; }
protected:
    virtual void run(); 
private slots:
    void update_play();
private:
    std::mutex  m_mutex;
    std::queue<AVFrame*> m_frame_cache;   // 视频播放缓存区
    bool m_start = false;
    bool m_pause = false;
    VideoPlayInterface *m_play = nullptr;
    QTimer m_playTimer;     // 播放画面的定时器
    int64_t m_syncPts = 0;  // 同步时间，由外部传入的音频pts
    int64_t m_pts = 0;      // 当前播放的pts
};