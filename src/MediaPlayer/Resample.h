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

    // 输出参数和输入参数一致 除了采样格式 输出为s16
    bool open(AVCodecParameters *param);
    void close();
    //void clear();

    // 返回重采样后大小, 不管成功与否都释放indata空间
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

