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
    std::lock_guard<std::mutex> lck(m_mutex);

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
    start();    // ��ʼqt�߳�

    return true;
}

void VideoThread::close()
{
    m_mutex.lock();
    if (m_decode)
    {
        delete m_decode;
        m_decode = nullptr;
    }
    clearCache();

    m_play = nullptr;
    m_start = false;
    m_mutex.unlock();

    wait();     // �ȴ�QT�߳̽���
}

void VideoThread::clear()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    // ������뻺��
    if (m_decode)
    {
        m_decode->clear();
    }
    clearCache();
}

bool VideoThread::repaintPts(AVPacket * pkt, int64_t& seekpts)
{
    std::lock_guard<std::mutex> lck(m_mutex);

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
    //m_mutex.lock();
    m_pause = pause;
    //m_mutex.unlock();
}

void VideoThread::run()
{
    while (m_start)
    {
        m_mutex.lock();

        // double check
        if (!m_start)
        {
            m_mutex.unlock();
            break;
        }
        if (m_pause)
        {
            m_mutex.unlock();
            msleep(10);
            continue;
        }

        // ����Ƶͬ��, ����Ƶ��һ֡����ʱ��С����һ֡��Ƶ
        if (m_syncPts > 0 && m_syncPts < m_decode->pts())
        {
            m_mutex.unlock();
            msleep(1);
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
            msleep(1);
            continue;
        }

        // һ��send���recv
        while (true)
        {
            // ��ȡһ֡
            AVFrame *frame = m_decode->recv();
            if (!frame)
            {
                LOG(DEBUG) << "m_decode->recv is failed!";
                break;
            }

            // ��ʾ��Ƶ,�����Ժ�ʱ����1ms
            m_play->repaint(frame);

            // �ͷ�һ֡��Դ
            Decode::freeFrame(&frame);
        }
        m_mutex.unlock();
        msleep(1);  // ������Դռ�ù���
    }
}