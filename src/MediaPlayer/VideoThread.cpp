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
    clearCache();
}

bool VideoThread::repaintPts(AVPacket * pkt, int64_t& seekpts)
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

void VideoThread::run()
{
    while (m_start)
    {
        if (m_pause)
        {
            msleep(10);
            continue;
        }
        
        m_mutex.lock();

        // 音视频同步, 若音频上一帧播放时间小于上一帧视频
        if (m_syncPts > 0 && m_syncPts < m_decode->pts())
        {
            m_mutex.unlock();
            usleep(1);
            continue;
        }

        AVPacket *pkt = popPacket();
        if (!pkt)
        {
            m_mutex.unlock();
            LOG(DETAIL) << "packet queue is empty! wait for 1ms";
            msleep(1);
            continue;
        }

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
            AVFrame *frame = m_decode->recv();
            if (!frame)
            {
                LOG(DEBUG) << "m_decode->recv is failed!";
                break;
            }

            // 显示视频,经测试耗时不到1ms
            m_play->repaint(frame);

            // 释放一帧资源
            Decode::freeFrame(&frame);
        }
        m_mutex.unlock();
    }
    LOG(INFO) << "video decode thread stop!";
}