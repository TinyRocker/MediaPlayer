#include "Decode.h"
#include "FFmpegUtils.h"
#include "glog/logging.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

Decode::Decode()
{
}

Decode::~Decode()
{
}

bool Decode::open(AVCodecParameters * param)
{
    if (!param)
    {
        LOG(ERROR) << "param is null!";
        return false;
    }

    close();

    std::lock_guard<std::mutex> lck(m_mutex);

    m_codecParam = param;
    LOG(INFO) << "codec name:" << avcodec_descriptor_get(m_codecParam->codec_id)->name;

    // 1.找到解码器
    AVCodec *codec = avcodec_find_decoder(m_codecParam->codec_id);
    if (!codec)
    {
        LOG(ERROR) << "avcodec_find_decoder failed! codec name:" << avcodec_descriptor_get(m_codecParam->codec_id)->name;
        return false;
    }

    // 2.配置解码器上下文参数
    m_codecCtx = avcodec_alloc_context3(codec);
    if (!m_codecCtx)
    {
        LOG(ERROR) << "avcodec_alloc_context3 failed!";
        return false;
    }
    if (AVMEDIA_TYPE_VIDEO == m_codecCtx->codec_type)
    {
        m_codecCtx->thread_count = std::thread::hardware_concurrency();
    }
    
    int ret = avcodec_parameters_to_context(m_codecCtx, m_codecParam);
    if (ret < 0)
    {
        avcodec_free_context(&m_codecCtx);
        LOG(ERROR) << "avcodec_parameters_to_context failed! reason:" << errstr(ret);
        return false;
    }

    // 3.打开解码器上下文
    ret = avcodec_open2(m_codecCtx, codec, nullptr);
    if (ret < 0)
    {
        avcodec_free_context(&m_codecCtx);
        LOG(ERROR) << "avcodec_open2 failed! reason:" << errstr(ret);
        return false;
    }

    return true;
}

void Decode::close()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (m_codecParam)
    {
        avcodec_parameters_free(&m_codecParam);
        m_codecParam = nullptr;
    }

    if (m_codecCtx)
    {
        avcodec_close(m_codecCtx);
        avcodec_free_context(&m_codecCtx);
        m_codecCtx = nullptr;
    }
}

void Decode::clear()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (m_codecCtx)
    {
        avcodec_flush_buffers(m_codecCtx);
    }
}

bool Decode::send(AVPacket * pkt)
{
    // 容错处理
    if (!pkt || pkt->size <= 0 || !pkt->data)
    {
        LOG(ERROR) << "param error!";
        return false;
    }

    std::lock_guard<std::mutex> lck(m_mutex);
    if (!m_codecCtx)
    {
        LOG(ERROR) << "avcodec not open!";
        return false;
    }

    int ret = avcodec_send_packet(m_codecCtx, pkt);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_send_packet failed! reason:" << errstr(ret);
        return false;
    }

    return true;
}

AVFrame * Decode::recv()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (!m_codecCtx)
    {
        LOG(ERROR) << "avcodec not open!";
        return false;
    }

    AVFrame *frame = av_frame_alloc();
    if (!frame)
    {
        LOG(ERROR) << "av_frame_alloc failed!";
        return nullptr;
    }

    int ret = avcodec_receive_frame(m_codecCtx, frame);
    if (ret < 0)  
    {
        av_frame_free(&frame);

        // Resource temporarily unavailable  errorcode -11 | ffmepeg v 4.1.3
        if (ret != -11)
        {
            LOG(ERROR) << "avcodec_receive_frame failed! reason:" << errstr(ret);
        }
        return nullptr;
    }
    m_pts = frame->pts;     // 保存解码帧的pts

    return frame;
}

void Decode::freeFrame(AVFrame **frame)
{
    if (frame && *frame)
    {
        av_frame_free(frame);
    }
}

void Decode::freePacket(AVPacket **packet)
{
    if (packet && *packet)
    {
        av_packet_free(packet);
    }
}