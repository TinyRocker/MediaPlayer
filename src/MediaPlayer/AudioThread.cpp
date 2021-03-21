#include "AudioThread.h"
#include "Decode.h"
#include "Resample.h"
#include "AudioPlay.h"
#include "glog/logging.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

AudioThread::AudioThread()
{
}

AudioThread::~AudioThread()
{
}

bool AudioThread::open(AVCodecParameters *param)
{
    if (!param)
    {
        LOG(ERROR) << "param is null!";
        return false;
    }

    if (!m_decode)
    {
        m_decode = new Decode;
    }
    if (!m_resample)
    {
        m_resample = new Resample;
    }
    if (!m_play)
    {
        m_play = new AudioPlay;
    }
    
    if (!m_decode->open(param))
    {
        LOG(ERROR) << "audio Decode open failed!";
        return false;
    }
    if (!m_resample->open(param))
    {
        LOG(ERROR) << "audio Resample open failed!";
        return false;
    }
    if (!m_play->open(m_resample->sampleRate(), m_resample->sampleSize(), m_resample->channels()))
    {
        LOG(ERROR) << "AudioPlay open failed!";
        return false;
    }
    m_pts = 0;
    m_pause = false;
    m_start = true;
    
    start();    // 开始qt线程

    return true;
}

void AudioThread::close()
{
    m_start = false;

    m_mutex.lock();
    if (m_decode)
    {
        m_decode->close();
        delete m_decode;
        m_decode = nullptr;
    }

    if (m_resample)
    {
        m_resample->close();
        delete m_resample;
        m_resample = nullptr;
    }

    if (m_play)
    {
        m_play->close();
        delete m_play;
        m_play = nullptr;
    }
    m_mutex.unlock();

    clearCache();    // 清理队列缓冲
    wait();     // 等待qt线程退出
}

void AudioThread::clear()
{
    if (m_decode)
    {
        m_decode->clear();
    }
    if (m_play)
    {
        m_play->clear();
    }
    clearCache();
}

void AudioThread::setPause(bool pause)
{
    m_pause = pause;    // 将pause优先级设高，已实现在另外一个线程中修改值可以立即生效

    if (m_play)
    {
        m_play->setPause(m_pause);
    }
}

void AudioThread::setVolumeValue(double num)
{
    if (m_play)
    {
        m_play->setVolumeValue(num);
    }
}

bool AudioThread::playToPts(AVPacket* pkt, int64_t& seekpts)
{
    bool ret = m_decode->send(pkt);
    if (!ret)
    {
        LOG(ERROR) << "decode send failed!";
        return false;
    }

    while (true)
    {
        AVFrame* frame = m_decode->recv();
        if (!frame)
        {
            return false;
        }
        else if (frame->pts >= seekpts)
        {
            seekpts = frame->pts;
            Decode::freeFrame(&frame);
            return true;
        }
        else
        {
            Decode::freeFrame(&frame);
        }
    }
}

void AudioThread::run()
{
    uint8_t *pcm = new uint8_t[1024 * 1024 * 10];   // 分配10M空间

    while (m_start)
    {
        if (m_pause)
        {
            msleep(10);
            continue;
        }

        AVPacket *pkt = popPacket();
        if (!pkt)
        {
            LOG(DETAIL) << "packet queue is empty!";
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
            AVFrame *frame = m_decode->recv();
            if (!frame)
            {
                LOG(DEBUG) << "m_decode->recv is failed!";
                break;
            }

            // 当前解码的pts减去缓冲中未播放的时间就是已经播放的时长
            m_pts = m_decode->pts() - m_play->noPlayMs();

            // 开始重采样
            int size = m_resample->resample(frame, pcm);
            Decode::freeFrame(&frame); // 释放资源
            while (size > 0 && !m_pause)
            {
                // 缓冲未播放完，空间不够
                if (m_play->freeSpaceSize() < size)
                {
                    msleep(1);
                    continue;
                }
                else
                {
                    m_play->writeData((const char *)pcm, size);
                    break;
                }
            }
        }
        m_mutex.unlock();
    }
    delete pcm;

    LOG(INFO) << "audio decode thread stop!";
}
