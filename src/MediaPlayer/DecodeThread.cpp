#include "DecodeThread.h"
#include "Decode.h"
#include "glog/logging.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

DecodeThread::DecodeThread()
{
}

DecodeThread::~DecodeThread()
{
}

void DecodeThread::pushPacket(AVPacket *pkt)
{
    if (!pkt)
    {
        LOG(WARNING) << "param is null!";
        return;
    }

    // 阻塞直至写入成功
    while (true)
    {
        m_queLck.lock();
        if (m_pktQue.size() < m_cacheSize)
        {
            m_pktQue.push(pkt);
            m_queLck.unlock();
            break;
        }
        m_queLck.unlock();
        msleep(1);
    }
}

AVPacket *DecodeThread::popPacket()
{
    AVPacket *pkt = nullptr;

    m_queLck.lock();
    if (!m_pktQue.empty())
    {
        pkt = m_pktQue.front();
        m_pktQue.pop();
    }
    m_queLck.unlock();

    return pkt;
}

void DecodeThread::clearCache()
{
    m_queLck.lock();

    // 清理队列中的AVPacket
    while (!m_pktQue.empty())
    {
        AVPacket *pkt = m_pktQue.front();
        m_pktQue.pop();
        Decode::freePacket(&pkt);
    }
    m_queLck.unlock();
}
