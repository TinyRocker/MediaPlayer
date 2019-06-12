#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>
#include "VideoPlayInterface.h"

class VideoWidget :
    public QOpenGLWidget,
    public QOpenGLFunctions,
    public VideoPlayInterface
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent);
    ~VideoWidget();

    virtual bool initWidget(const int width, const int height);
    virtual bool deinitWidget();
    
    // repaint, 显示一帧数据
    virtual void repaint(const AVFrame *frame);

    int videoWidth() const { return m_width; }
    int videoHeight() const { return m_height; }
protected:
    // 初始化gl
    void initializeGL();
    // 窗口尺寸变化
    void resizeGL(int w, int h);
    // 刷新显示
    void paintGL();
private:
    std::mutex m_mutex;
    QGLShaderProgram m_shader;       //shader程序
    GLuint m_unis[3] = { 0 };        //shader中的yuv变量地址
    GLuint m_texs[3] = { 0 };        //opengl的texture地址
    uint8_t *m_yuv[3] = { nullptr }; //材质的内存空间

    int m_width = 800;
    int m_height = 500;
};