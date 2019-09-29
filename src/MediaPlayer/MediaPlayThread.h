#pragma once
#include <QThread>
#include <string>
#include <mutex>

class Demux;
class VideoThread;
class AudioThread;
class VideoPlayInterface;

class MediaPlayThread : protected QThread
{
public:
    MediaPlayThread();
    virtual ~MediaPlayThread();

    bool open(const char *url, VideoPlayInterface *videoInter);
    void close();
    void clear();
    void setPause(bool pause);
    void seek(double pos);
    void setVolumeValue(double num);

    bool pause() const { return m_pause; };
    int64_t pts() const { return m_pts; }
    int64_t totalMs() const { return m_totalMs; };
    int width() const;
    int height() const;
protected:
    virtual void run();
private:
    std::mutex      m_mutex;
    std::string     m_url;
    bool            m_start = false;
    Demux           *m_demux = nullptr;
    VideoThread     *m_video = nullptr;
    AudioThread     *m_audio = nullptr;

    bool    m_pause = false;    // 暂停播放
    int64_t m_pts = 0;          // 当前播放时间
    int64_t m_totalMs = 0;      // 文件总时长
};

