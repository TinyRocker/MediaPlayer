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

    std::lock_guard<std::mutex> lck(m_mutex);

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
    
    start();    // ��ʼqt�߳�

    return true;
}

void AudioThread::close()
{
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

    clearCache();    // ������л���
    m_start = false;
    m_mutex.unlock();

    wait();     // �ȴ�qt�߳��˳�
}

void AudioThread::clear()
{
    std::lock_guard<std::mutex> lck(m_mutex);

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
    m_pause = pause;    // ��pause���ȼ���ߣ���ʵ��������һ���߳����޸�ֵ����������Ч

    std::lock_guard<std::mutex> lck(m_mutex);
    if (m_play)
    {
        m_play->setPause(m_pause);
    }
}

void AudioThread::setVolumeValue(double num)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (m_play)
    {
        m_play->setVolumeValue(num);
    }
}

void AudioThread::run()
{
    uint8_t *pcm = new uint8_t[1024 * 1024 * 10];   // ����10M�ռ�

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

        AVPacket *pkt = popPacket();
        if (!pkt)
        {
            m_mutex.unlock();
            LOG(DETAIL) << "packet queue is empty!";
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
        while (m_start && !m_pause)
        {
            AVFrame *frame = m_decode->recv();
            if (!frame)
            {
                LOG(DEBUG) << "m_decode->recv is failed!";
                break;
            }

            // ��ǰ�����pts��ȥ������δ���ŵ�ʱ��
            m_pts = m_decode->pts() - m_play->noPlayMs();

            // ��ʼ�ز���
            int size = m_resample->resample(frame, pcm);
            Decode::freeFrame(&frame); // �ͷ���Դ
            while (size > 0 && !m_pause)
            {
                // ����δ�����꣬�ռ䲻��
                if (m_play->freeSpaceSize() < size)
                {
                    LOG(DETAIL) << "freeSpaceSize is not enough!";
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
        msleep(1);
    }
    delete pcm;
}
