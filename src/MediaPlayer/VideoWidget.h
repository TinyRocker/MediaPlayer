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
    
    // repaint, ��ʾһ֡����
    virtual void repaint(const AVFrame *frame);

    int videoWidth() const { return m_width; }
    int videoHeight() const { return m_height; }
protected:
    // ��ʼ��gl
    void initializeGL();
    // ���ڳߴ�仯
    void resizeGL(int w, int h);
    // ˢ����ʾ
    void paintGL();
private:
    std::mutex m_mutex;
    QGLShaderProgram m_shader;       //shader����
    GLuint m_unis[3] = { 0 };        //shader�е�yuv������ַ
    GLuint m_texs[3] = { 0 };        //opengl��texture��ַ
    uint8_t *m_yuv[3] = { nullptr }; //���ʵ��ڴ�ռ�

    int m_width = 800;
    int m_height = 500;
};