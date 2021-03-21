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

    // 初始化video widget
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
    // 视频或音频至少存在一个
    if (!m_video && !m_audio)
    {
        return false;
    }
    m_totalMs = m_demux->totalMs();
    m_pause = false;
    m_start = true;
    start();    // 开启QT线程

    return true;
}

void MediaPlayThread::close()
{
    m_start = false;

    m_mutex.lock();
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

    wait();     // 等待qt线程退出  
}

void MediaPlayThread::clear()
{
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
    // 获取播放状态
    const bool status = m_pause;

    // 1.若未暂停，先暂停播放
    if (!m_pause)
    {
        setPause(true);
    }

    // 2.清理缓冲
    clear();

    m_mutex.lock();
    if (m_demux)
    {
        // seek 到指定位置帧的前一个 I帧处
        m_demux->seek(pos);

        // 跳过指定帧前面的帧
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

    // 将播放状态还原
    if (status != m_pause)
    {
        setPause(status);
    }
}

void MediaPlayThread::setVolumeValue(double num)
{
    if (m_audio)
    {
        m_audio->setVolumeValue(num);
    }
}

int MediaPlayThread::width() const
{
    if (!m_demux)
    {
        return 0;
    }
    return m_demux->width();
}

int MediaPlayThread::height() const
{
    if (!m_demux)
    {
        return 0;
    }
    return m_demux->height();
}

// 读取音视频packet，存入队列
void MediaPlayThread::run()
{
    while (m_start)
    {
        if (m_pause)
        {
            msleep(10);
            continue;
        }

        m_mutex.lock();

        // 音视频同步
        if (m_video && m_audio)
        {
            m_pts = m_audio->pts();
            m_video->syncPts(m_audio->pts());
        }
        
        AVPacket *pkt = m_demux->read();
        if (!pkt)
        {
            m_mutex.unlock();
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

        // TODO
        // 不睡眠会引起高cpu占用率，但是由于未做解码缓存，睡眠会导致播放卡顿
        // usleep(1);   
    }

    LOG(INFO) << "stop media thread! url:" << m_url.c_str();
}