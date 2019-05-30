#include "MediaPlayThread.h"
#include "Demux.h"
#include "VideoThread.h"
#include "AudioThread.h"
#include "VideoPlayInterface.h"
#include "glog/logging.h"

#include "Decode.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

MediaPlayThread::MediaPlayThread()
{
}

MediaPlayThread::~MediaPlayThread()
{
}

bool MediaPlayThread::open(const char * url, VideoPlayInterface *videoInter)
{
    close();

    std::lock_guard<std::mutex> lck(m_mutex);

    if (!url || !videoInter)
    {
        LOG(ERROR) << "param is null!";
        return false;
    }

    m_url = url;
    if (!m_demux)
    {
        m_demux = new Demux;
    }
    if (!m_video)
    {
        m_video = new VideoThread;
    }
    if (!m_audio)
    {
        m_audio = new AudioThread;
    }

    if (!m_demux->open(url))
    {
        LOG(ERROR) << "m_demux->open:" << url << " failed!";
        return false;
    }

    // ��ʼ��video widget
    videoInter->initWidget(m_demux->width(), m_demux->height());
    if (m_video && !m_video->open(m_demux->getVideoCodecParam(), videoInter))
    {
        LOG(WARNING) << "video thread open failed!";
        delete m_video;
        m_video = nullptr;
    }
    if (m_audio && !m_audio->open(m_demux->getAudioCodecParam()))
    {
        LOG(WARNING) << "audio thread open failed!";
        delete m_audio;
        m_audio = nullptr;
    }
    // ��Ƶ����Ƶ���ٴ���һ��
    if (!m_video && !m_audio)
    {
        return false;
    }
    m_totalMs = m_demux->totalMs();
    m_pause = false;
    m_start = true;
    start();    // ����QT�߳�

    return true;
}

void MediaPlayThread::close()
{
    m_mutex.lock();
    m_start = false; 
    if (m_video)
    {
        m_video->close();
        delete m_video;
        m_video = nullptr;
    }
    if (m_audio)
    {
        m_audio->close();
        delete m_audio;
        m_audio = nullptr;
    }
    if (m_demux)
    {
        m_demux->close();
        delete m_demux;
        m_demux = nullptr;
    }
    m_mutex.unlock();

    wait();     // �ȴ�qt�߳��˳�  
}

void MediaPlayThread::clear()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (m_demux)
    {
        m_demux->clear();
    }
    if (m_video)
    {
        m_video->clear();
    }
    if (m_audio)
    {
        m_audio->clear();
    }
}

void MediaPlayThread::setPause(bool pause)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    m_pause = pause;
    if (m_video)
    {
        m_video->setPause(m_pause);
    }
    if (m_audio)
    {
        m_audio->setPause(m_pause);
    }
}

void MediaPlayThread::seek(double pos)
{
    // ��ȡ����״̬
    const bool status = m_pause;

    // 1.��δ��ͣ������ͣ����
    if (!m_pause)
    {
        setPause(true);
    }

    // 2.������
    clear();

    m_mutex.lock();
    if (m_demux)
    {
        // seek ��ָ��λ��֡��ǰһ�� I֡��
        m_demux->seek(pos);

        // ����ָ��֡ǰ���֡
        int64_t seek_pts = pos * m_totalMs;
        while (m_start)
        {
            if (m_video)
            {
                AVPacket *pkt = m_demux->readVideoOnly();
                bool ret = m_video->repaintPts(pkt, seek_pts);
                Demux::freePacket(&pkt);
                if (ret)
                {
                    m_pts = seek_pts;
                    break;
                }
            }
            else if (m_audio)
            {
                // TODO
                LOG(WARNING) << "not realize audio seek!";
                break;
            }
            else
            {
                LOG(ERROR) << "not found video or audio packet!";
                break;
            }
        }
    }
    m_mutex.unlock();

    // ������״̬��ԭ
    if (status != m_pause)
    {
        setPause(status);
    }
}

void MediaPlayThread::setVolumeValue(double num)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (m_audio)
    {
        m_audio->setVolumeValue(num);
    }
}

void MediaPlayThread::run()
{
    while (m_start)
    {
        m_mutex.lock();

        //double check
        if (!m_start)
        {
            m_mutex.unlock();
            LOG(INFO) << "stop media thread! url:" << m_url.c_str();
            break;
        }

        if (m_pause)
        {
            m_mutex.unlock();
            msleep(10);
            continue;
        }
        
        // ����Ƶͬ��
        if (m_video && m_audio)
        {
            m_pts = m_audio->pts();
            m_video->syncPts(m_audio->pts());
        }

        AVPacket *pkt = m_demux->read();
        if (!pkt)
        {
            m_mutex.unlock();
            msleep(1);
            continue;
        }
        else if (m_audio && m_demux->isAudio(pkt))
        {
            m_audio->pushPacket(pkt);
        }
        else if (m_video && m_demux->isVideo(pkt))
        {
            m_video->pushPacket(pkt);
        }
        m_mutex.unlock();
        usleep(1);
    }
}