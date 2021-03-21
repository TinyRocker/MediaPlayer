#include "Demux.h"
#include "FFmpegUtils.h"
#include "glog/logging.h"
#include <QThread>

extern "C"
{
#include "libavformat/avformat.h"
}

Demux::Demux()
{
}

Demux::~Demux()
{
}

bool Demux::open(const char * url)
{
    close();

    m_url = url;
    
    // 1. 打开url
    int ret = avformat_open_input(&m_ic, url, nullptr, nullptr);
    if (ret < 0)
    {
        LOG(ERROR) << "avformat_open_input " << url << " failed! reason:" << errstr(ret);
        return false;
    }
    LOG(INFO) << "avformat_open_input " << url << " success!";

    // 2.获取流信息
    ret = avformat_find_stream_info(m_ic, nullptr);
    if (ret < 0)
    {
        LOG(ERROR) << "avformat_find_stream_info failed! reason:" << errstr(ret);
        return false;
    }

    m_totalMs = m_ic->duration / (AV_TIME_BASE / 1000); // 毫秒
    int64_t totals = m_ic->duration / AV_TIME_BASE; // duration / AV_TIME_BASE 得到秒
    m_hours = totals / 3600;
    m_minutes = (totals % 3600) / 60;
    m_seconds = totals % 60;
    LOG(INFO) << "meida time: " << m_hours << ":" << m_minutes << ":" << m_seconds;

    /// 打印流信息
    std::cout << "====================stream info==========================" << std::endl;
    av_dump_format(m_ic, 0, url, 0);
    std::cout << "=========================================================" << std::endl;
    
    // 3.获取视频流信息（函数获取）
    m_vIndex = av_find_best_stream(m_ic, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (m_vIndex >= 0)
    {
        m_vs = m_ic->streams[m_vIndex];
        m_width = m_vs->codecpar->width;
        m_height = m_vs->codecpar->height;

        LOG(INFO) << "------------视频信息: " << m_vIndex << " ------------";
        LOG(INFO) << "codec_id = " << m_vs->codecpar->codec_id;
        LOG(INFO) << "format = " << m_vs->codecpar->format;
        LOG(INFO) << "" << m_width << " * " << m_height;
        LOG(INFO) << "video fps = " << r2d(m_vs->avg_frame_rate);  ///帧率要做分数转换
    }
    
    // 4.获取音频流信息（函数获取）
    m_aIndex = av_find_best_stream(m_ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (m_aIndex >= 0)
    {
        m_as = m_ic->streams[m_aIndex];

        // 一帧数据 1024 * 6(通道数) * 2(16bit) = 12288, fps = sample_rate / 12288
        m_sampleRate = m_as->codecpar->sample_rate;
        m_channels = m_as->codecpar->channels;
        LOG(INFO) << "------------音频信息: " << m_aIndex << " ------------";
        LOG(INFO) << "codec_id = " << m_as->codecpar->codec_id;
        LOG(INFO) << "format = " << m_as->codecpar->format;
        LOG(INFO) << "sample_rate = " << m_as->codecpar->sample_rate;
        LOG(INFO) << "channels = " << m_as->codecpar->channels;
        LOG(INFO) << "frame size= " << m_as->codecpar->frame_size;
    }

    if (!m_as && !m_vs)
    {
        LOG(ERROR) << "not find video and audio stream!";
        return false;
    }

    return true;
}

void Demux::close()
{
    if (m_ic)
    {
        avformat_close_input(&m_ic);
        m_ic = nullptr;
    }
    m_as = nullptr;
    m_vs = nullptr;
    m_totalMs = 0;
}

void Demux::clear()
{
    if (m_ic)
    {
        // 清理读取缓冲
        avformat_flush(m_ic);
    }
}

AVPacket *Demux::read()
{
    if (!m_ic)
    {
        LOG(ERROR) << "AVFormatContext not open!";
        return nullptr;
    }

    AVPacket *pkt = av_packet_alloc();
    if (!pkt)
    {
        LOG(ERROR) << "av_packet_alloc failed!";
        return nullptr;
    }

    // 读取一帧
    int ret = av_read_frame(m_ic, pkt);
    if (ret < 0)
    {
        av_packet_free(&pkt);
        LOG(ERROR) << "av_read_frame failed! reason:" << errstr(ret);
        if (strcmp(errstr(ret).c_str(), "End of file") == 0)
        {
        }
        return nullptr;
    }

    // pts转换为毫秒
    av_packet_rescale_ts(pkt, m_ic->streams[pkt->stream_index]->time_base, AVRational{ 1, 1000 });

    return pkt;
}

AVPacket * Demux::readVideoOnly()
{
    AVPacket *pkt = nullptr;

    // 防止阻塞
    for (int i = 0; i < 1000; ++i)
    {
        pkt = read();
        if (!pkt)
        {
            return nullptr;
        }
        else if (isVideo(pkt))
        {
            return pkt;
        }
        else
        {
            Demux::freePacket(&pkt);
        }
    }
    return nullptr;
}

AVPacket * Demux::readAudioOnly()
{
    AVPacket *pkt = nullptr;

    // 防止阻塞
    for (int i = 0; i < 1000; ++i)
    {
        pkt = read();
        if (!pkt)
        {
            return nullptr;
        }
        else if (isAudio(pkt))
        {
            return pkt;
        }
        else
        {
            Demux::freePacket(&pkt);
        }
    }
    return nullptr;
}

void Demux::freePacket(AVPacket ** packet)
{
    if (packet && *packet)
    {
        av_packet_free(packet);
    }
}

bool Demux::seek(double pos)
{
    if (!m_ic)
    {
        LOG(ERROR) << "AVFormatContext not open!";
        return false;
    }

    int64_t seekPos = 0;
    int index = -1; // 默认使用文件索引

    // 清理读取缓冲
    avformat_flush(m_ic);
    if (m_ic->duration > 0)    // 如果文件时长有效
    {
        seekPos = m_ic->duration * pos;
        index = -1;
    }
    else if (m_vs)
    {
        seekPos = m_vs->duration * pos;
        index = m_vIndex;
    }
    else if (m_as)
    {
        seekPos = m_as->duration * pos;
        index = m_aIndex;
    }
    else
    {
        LOG(ERROR) << "file time invalid, video and audio stream invalid!";
        return false;
    }

    int ret = av_seek_frame(m_ic, index, seekPos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);  // 向前找关键帧
    if (ret < 0)
    {
        LOG(ERROR) << "av_seek_frame failed! reason:" << errstr(ret);
        return false;
    }
    return true;
}

AVCodecParameters* Demux::getCodecParam(AVStream *stream)
{
    AVCodecParameters *param = avcodec_parameters_alloc();
    if (!param)
    {
        LOG(ERROR) << "avcodec_parameters_alloc failed!";
        return nullptr;
    }
    int ret = avcodec_parameters_copy(param, stream->codecpar);
    if (ret < 0)
    {
        LOG(WARNING) << "avcodec_parameters_copy failed! reason:" << errstr(ret);
        avcodec_parameters_free(&param);
        return nullptr;
    }
    return param;
}

void Demux::freeCodecParam(AVCodecParameters **param)
{
    if (param && *param)
    {
        avcodec_parameters_free(param);
    }
}

AVCodecParameters *Demux::getVideoCodecParam()
{
    if (!m_vs)
    {
        LOG(WARNING) << "video stream is not exist!";
        return nullptr;
    }
    return getCodecParam(m_vs);
}

AVCodecParameters *Demux::getAudioCodecParam()
{
    if (!m_as)
    {
        LOG(WARNING) << "audio stream is not exist!";
        return nullptr;
    }
    return getCodecParam(m_as);
}

bool Demux::isVideo(const AVPacket * pkt)
{
    return pkt && pkt->stream_index == m_vIndex;
}

bool Demux::isAudio(const AVPacket * pkt)
{
    return pkt && pkt->stream_index == m_aIndex;
}