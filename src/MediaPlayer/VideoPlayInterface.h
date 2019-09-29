#pragma once

class AVFrame;

// 视频播放接口类
class VideoPlayInterface
{
public:
    virtual bool initWidget(const int width, const int height) = 0;
    virtual bool deinitWidget() = 0;
    virtual void repaint(const AVFrame *frame) = 0;
};