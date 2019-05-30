#include "Resample.h"
#include "FFmpegUtils.h"
#include "glog/logging.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
}

Resample::Resample()
{
}

Resample::~Resample()
{
}

bool Resample::open(AVCodecParameters * param)
{
    close();

    std::lock_guard<std::mutex> lck(m_mutex);

    if (!param) { return false; }
    m_codecParam = param;
    m_sampleRate = param->sample_rate;
    m_sampleSize = 16;
    m_channels = param->channels;

    // 音频重采样 上下文初始化,如果actx_为NULL会分配空间
    m_swrCtx_ = swr_alloc_set_opts(
        m_swrCtx_,
        av_get_default_channel_layout(param->channels),  // 输出格式
        AV_SAMPLE_FMT_S16,                  // 输出样本格式
        param->sample_rate,                 // 输出采样率
        av_get_default_channel_layout(param->channels),  // 输入格式
        (AVSampleFormat)param->format,
        param->sample_rate,
        0,
        nullptr);

    int ret = swr_init(m_swrCtx_);
    if (ret < 0)
    {
        LOG(ERROR) << "swr_init failed! reason:" << errstr(ret);
        swr_free(&m_swrCtx_);
        return false;
    }

    return true;
}

void Resample::close()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (m_codecParam)
    {
        //AVCodecParameters *tmp = m_codecParam;
        //avcodec_parameters_free(&tmp);
        m_codecParam = nullptr;
    }

    if (m_swrCtx_)
    {
        swr_free(&m_swrCtx_);
        m_swrCtx_ = nullptr;
    }
}

int Resample::resample(AVFrame * indata, unsigned char * outdata)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (!indata)
    {
        LOG(ERROR) << "input param error!";
        return 0;
    }
    if (!outdata)
    {
        LOG(ERROR) << "output param error!";
        return 0;
    }

    uint8_t *data[2] = { nullptr };
    data[0] = outdata;
    int ret = swr_convert(
        m_swrCtx_,
        data,
        indata->nb_samples,             // 输出
        (const uint8_t **)indata->data, // 输入
        indata->nb_samples);
    if (ret < 0)
    {
        LOG(ERROR) << "swr_convert failed! reason:" << errstr(ret);
        return 0;
    }

    return ret * indata->channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
}