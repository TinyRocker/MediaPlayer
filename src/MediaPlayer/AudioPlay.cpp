#include "AudioPlay.h"
#include "glog/logging.h"
#include <QAudioFormat>
#include <QAudioOutput>

AudioPlay::AudioPlay()
{
}

AudioPlay::~AudioPlay()
{
}

bool AudioPlay::open(int sampleRate, int sampleSize, int channels)
{
    close();

    std::lock_guard<std::mutex> lck(m_mutex);

    m_sampleRate = sampleRate;
    m_sampleSize = sampleSize;
    m_channels = channels;

    QAudioFormat fmt;
    fmt.setSampleRate(m_sampleRate);
    fmt.setSampleSize(m_sampleSize);
    fmt.setChannelCount(m_channels);
    fmt.setCodec("audio/pcm");
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setSampleType(QAudioFormat::UnSignedInt);

    m_output = new QAudioOutput(fmt);
    m_io = m_output->start();   // 开始播放
    if (!m_io)
    {
        LOG(ERROR) << "QAudioOutput start failed!";
        return false;
    }

    return true;
}

void AudioPlay::close()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (m_io)
    {
        m_io->close();  // 关闭io
        m_io = nullptr;
    }
    if (m_output)
    {
        m_output->stop();
        delete m_output;
        m_output = nullptr;
    }
}

void AudioPlay::clear()
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (m_io)
    {
        m_io->reset();
    }
}

bool AudioPlay::writeData(const char *data, int datasize)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (!data || datasize <= 0)
    {
        LOG(ERROR) << "param error!";
        return false;
    }
    if (!m_output || !m_io)
    {
        LOG(ERROR) << "QAudioOutput or QIODevice is null!";
        return false;
    }

    int size = m_io->write(data, datasize);
    if (size != datasize)
    {
        LOG(ERROR) << "QIODevice write failed! write size:" << size << ", data size:" << datasize;
        return false;
    }

    return true;
}

int AudioPlay::freeSpaceSize()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (!m_output)
    {
        return 0;
    }
    return m_output->bytesFree();
}

int64_t AudioPlay::noPlayMs()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (!m_output)
    {
        return 0;
    }
    
    double size = m_output->bufferSize() - m_output->bytesFree();       // 还未播放的字节数，buff - 空闲空间
    double secSize = m_sampleRate * (m_sampleSize / 8) * m_channels;    // 一秒音频的字节大小
    if (size <= 0 || secSize <= 0)
    {
        return 0;
    }
    return (size / secSize) * 1000;
}

void AudioPlay::setPause(bool pause)
{
    m_mutex.lock();
    m_pause = pause;
    if (m_output)
    {
        m_pause ? m_output->suspend() : m_output->resume();
    }
    m_mutex.unlock();
}

void AudioPlay::setVolumeValue(double num)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (m_output)
    {
        m_output->setVolume(num);
    }
}
