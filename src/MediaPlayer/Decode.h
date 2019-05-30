#pragma once
#include <mutex>

class AVCodecParameters;
class AVCodecContext;
class AVFrame;
class AVPacket;

class Decode
{
public:
    Decode();
    virtual ~Decode();

    // 打开解码器，并释放空间
    bool open(AVCodecParameters *param);
    void close();
    void clear();

    // 发送一个packet包到解码线程
    bool send(AVPacket *pkt);

    // 收到解码线程数据，一次send可能需要多次recv, 解码后的数据为AVFrame,空间为decode申请
    AVFrame* recv();

    // 释放 AVFrame AVPacket
    static void freeFrame(AVFrame **frame);
    static void freePacket(AVPacket **packet);

    long long pts() const { return m_pts; }
private:
    std::mutex m_mutex;
    AVCodecContext *m_codecCtx = nullptr;
    AVCodecParameters *m_codecParam = nullptr;
    long long m_pts = 0;    // 当前解码到的pts
};

