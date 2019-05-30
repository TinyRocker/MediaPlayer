#pragma once
#include <mutex>

class AVCodecParameters;
class SwrContext;
class AVFrame;

class Resample
{
public:
    Resample();
    virtual ~Resample();

    // ����������������һ�� ���˲�����ʽ ���Ϊs16
    bool open(AVCodecParameters *param);
    void close();
    //void clear();

    // �����ز������С, ���ܳɹ�����ͷ�indata�ռ�
    int resample(AVFrame *indata, unsigned char *outdata);

    int sampleRate() const { return m_sampleRate; }
    int sampleSize() const { return m_sampleSize; }
    int channels() const { return m_channels; }
private:
    std::mutex m_mutex;
    AVCodecParameters *m_codecParam = nullptr;
    SwrContext *m_swrCtx_ = nullptr;
    int m_sampleRate = 44100;
    int m_sampleSize = 16;
    int m_channels = 2;
};

