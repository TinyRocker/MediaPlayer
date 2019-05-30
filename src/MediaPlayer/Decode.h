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

    // �򿪽����������ͷſռ�
    bool open(AVCodecParameters *param);
    void close();
    void clear();

    // ����һ��packet���������߳�
    bool send(AVPacket *pkt);

    // �յ������߳����ݣ�һ��send������Ҫ���recv, ����������ΪAVFrame,�ռ�Ϊdecode����
    AVFrame* recv();

    // �ͷ� AVFrame AVPacket
    static void freeFrame(AVFrame **frame);
    static void freePacket(AVPacket **packet);

    long long pts() const { return m_pts; }
private:
    std::mutex m_mutex;
    AVCodecContext *m_codecCtx = nullptr;
    AVCodecParameters *m_codecParam = nullptr;
    long long m_pts = 0;    // ��ǰ���뵽��pts
};

