#pragma once
// 音视频解码显示线程的基类

#include <QThread>
#include <mutex>
#include <queue>

class AVPacket;
class Decode;

class DecodeThread : protected QThread
{
public:
    virtual ~DecodeThread();
    void pushPacket(AVPacket *pkt);
    AVPacket *popPacket();
    void clearCache();
    int cacheSize() const { return m_cacheSize; }
    void setCacheSize(int size) { m_cacheSize = size; }
    Decode *decodeHandle() const { return m_decode; }
protected:
    DecodeThread();
protected:
    Decode      *m_decode = nullptr;
private:
    std::queue <AVPacket*> m_pktQue;
    std::mutex  m_queLck;
    int m_cacheSize = 100;
};

