#pragma once
#include <string>

class AVFormatContext;
class AVCodecParameters;
class AVStream;
class AVPacket;

class Demux
{
public:
    Demux();
    virtual ~Demux();

    // 创建对象, 打开媒体文件或流媒体文件 rtsp rtmp
    bool open(const char *url);
    void close();
    void clear();

    // 返回AVPacket*空间, 释放，av_packet_free();
    AVPacket* read();
    AVPacket* readVideoOnly();
    AVPacket* readAudioOnly();

    static void freePacket(AVPacket **packet);

    // seek位置 pos 0.0 ~ 1.0 之间
    bool seek(double pos);

    static AVCodecParameters* getCodecParam(AVStream *stream);
    static void freeCodecParam(AVCodecParameters **param);

    // 获取音视频参数，返回空间需要清理，avcodec_parameters_free()
    AVCodecParameters *getVideoCodecParam();
    AVCodecParameters *getAudioCodecParam();

    bool isVideo(const AVPacket *pkt);
    bool isAudio(const AVPacket *pkt);

    int width() const { return m_width; }
    int height() const { return m_height; }
    int sampleRate() const { return m_sampleRate; }
    int channels() const { return m_channels; }

    int64_t totalMs() const { return m_totalMs; }
    int hour() const { return m_hours; }
    int minute() const { return m_minutes; }
    int second() const { return m_seconds; }
private:
    std::string m_url;
    AVFormatContext *m_ic = nullptr;
    AVStream *m_vs = nullptr;
    AVStream *m_as = nullptr;

    int64_t m_totalMs = 0;
    int m_hours = 0;
    int m_minutes = 0;
    int m_seconds = 0;

    int m_vIndex = -1;
    int m_aIndex = -1;

    int m_width = 0;
    int m_height = 0;

    int m_sampleRate = 0;
    int m_channels = 0;
};