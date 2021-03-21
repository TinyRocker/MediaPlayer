#include "VideoThread.h"
#include "Decode.h"
#include "VideoWidget.h"
#include "glog/logging.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

VideoThread::VideoThread()
{
    QObject::connect(&m_playTimer, SIGNAL(timeout()), this, SLOT(update_play()));
    m_playTimer.start(5);    // 5m刷新一次
}

VideoThread::~VideoThread()
{
}

bool VideoThread::open(AVCodecParameters *param, VideoPlayInterface *videoInter)
{
    if (!param || !videoInter)
    {
        LOG(ERROR) << "param is null!";
        return false;
    }

    if (!m_decode)
    {
        m_decode = new Decode;
    }

    if (!m_decode->open(param))
    {
        LOG(ERROR) << "audio Decode open failed!";
        return false;
    }

    m_play = videoInter;
    m_syncPts = 0;
    m_pause = false;
    m_start = true;
    start();    // 开始qt线程

    return true;
}

void VideoThread::close()
{
    m_start = false;

    m_mutex.lock();
    if (m_decode)
    {
        delete m_decode;
        m_decode = nullptr;
    }
    m_play = nullptr;
    m_mutex.unlock();

    clearCache();

    wait();     // 等待QT线程结束
}

void VideoThread::clear()
{
    // 清理解码缓冲
    if (m_decode)
    {
        m_decode->clear();
    }

    m_mutex.lock();
    // 清理已缓存的要播放的帧
    while (!m_frame_cache.empty())
    {
        AVFrame* frame = m_frame_cache.front();
        m_frame_cache.pop();
        Decode::freeFrame(&frame);
    }
    m_mutex.unlock();

    clearCache();
    m_pts = 0;  // pts清0
}

bool VideoThread::repaintToPts(AVPacket * pkt, int64_t& seekpts)
{
    bool ret = m_decode->send(pkt);
    if (!ret)
    {
        LOG(ERROR) << "decode send failed!";
        return false;
    }

    while (true)
    {
        AVFrame *frame = m_decode->recv();
        if (!frame)
        {
            return false;
        }
        else if (frame->pts >= seekpts)
        {
            seekpts = frame->pts;
            m_play->repaint(frame);
            Decode::freeFrame(&frame);
            return true;
        }
        else
        {
            Decode::freeFrame(&frame);
        }
    }
}

void VideoThread::setPause(bool pause)
{
    m_pause = pause;
}

// 解码线程
void VideoThread::run()
{
    while (m_start)
    {
        if (m_pause)
        {
            msleep(10);
            continue;
        }

        if (m_frame_cache.size() > 10)
        {
            // 缓存大于10帧 等待一下播放线程
            msleep(10);
            continue;
        }

        AVPacket *pkt = popPacket();
        if (!pkt)
        {
            LOG(DETAIL) << "packet queue is empty! wait for 1ms";
            msleep(10);
            continue;
        }

        m_mutex.lock();

        bool ret = m_decode->send(pkt);
        Decode::freePacket(&pkt);
        if (!ret)
        {
            m_mutex.unlock();
            LOG(WARNING) << "m_decode->send is failed!";
            continue;
        }

        // 一次send多次recv
        while (m_start && !m_pause)
        {
            // 获取一帧
            AVFrame *cache_frame = m_decode->recv();
            if (!cache_frame)
            {
                LOG(DEBUG) << "m_decode->recv is failed!";
                break;
            }
            m_frame_cache.push(cache_frame); // 缓存帧
        }
        m_mutex.unlock();
    }
    LOG(INFO) << "video decode thread stop!";
}

// 播放画面的定时器
void VideoThread::update_play()
{
    // 音视频同步 若视频播放过快 等待音频
    if (m_syncPts > 0 && m_syncPts < m_pts)
    {
        return;
    }

    m_mutex.lock();
    if (m_frame_cache.size() > 0)
    {
        AVFrame* frame = m_frame_cache.front();  // 播放帧
        m_frame_cache.pop();
        m_pts = frame->pts;         // 当前视频的

        if (m_play)
        {
            m_play->repaint(frame); // 显示视频,经测试耗时不到1ms
        }   
        Decode::freeFrame(&frame);  // 释放一帧资源
    }
    m_mutex.unlock();
}