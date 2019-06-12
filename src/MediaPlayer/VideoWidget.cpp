#include "VideoWidget.h"
#include "glog/logging.h"

extern "C"
{
#include "libavutil/frame.h"
}

//׼��yuv����
//ffmpeg -i LoL.mp4 -t 10 -s 240x128 -pix_fmt yuv420p out240x128.yuv

VideoWidget::VideoWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}

VideoWidget::~VideoWidget()
{
}

bool VideoWidget::initWidget(int width, int height)
{
    deinitWidget();

    std::lock_guard<std::mutex> lck(m_mutex);
    
    LOG(INFO) << "widget:" << width << ", height:" << height;
    if (width <= 0 || height <= 0)
    {
        LOG(ERROR) << "param error!";
        return false;
    }
    m_width = width;
    m_height = height;

    //��������ڴ�ռ� yuv420
    m_yuv[0] = new uint8_t[m_width * m_height];       ///Y
    m_yuv[1] = new uint8_t[m_width * m_height / 4];   ///U
    m_yuv[2] = new uint8_t[m_width * m_height / 4];   ///V
    if (!m_yuv[0] || !m_yuv[1] || !m_yuv[2])
    {
        LOG(ERROR) << "malloc memory failed!";
        return false;
    }

    // 5.��������
    glGenTextures(3, m_texs);

    //Y
    glBindTexture(GL_TEXTURE_2D, m_texs[0]);
    ///GL_NEAREST(�ٽ���ֵ��Ч�ʸߣ�������������) 
    ///GL_LINEAR (���Բ�ֵ��Ч�ʵͣ�����������)
    ///�Ŵ���ˣ����Բ�ֵ  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ///��С���ˣ����Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ///�������ʿռ�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //U
    glBindTexture(GL_TEXTURE_2D, m_texs[1]);
    ///�Ŵ���ˣ� ���Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ///��С���ˣ� ���Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ///�������ʿռ�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width / 2, m_height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //V
    glBindTexture(GL_TEXTURE_2D, m_texs[2]);
    ///�Ŵ���ˣ� ���Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ///��С���ˣ� ���Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ///���������Կ��ռ�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width / 2, m_height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    return true;
}

bool VideoWidget::deinitWidget()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    delete m_yuv[0];
    delete m_yuv[1];
    delete m_yuv[2];
    m_yuv[0] = nullptr;
    m_yuv[1] = nullptr;
    m_yuv[2] = nullptr;

    if (m_texs[0])
    {
        glDeleteTextures(3, m_texs);
    }

    return true;
}

void VideoWidget::repaint(const AVFrame * frame)
{
    if (!frame) return;

    // �ж�������
    m_mutex.lock();
    if (frame->width != m_width || frame->height != m_height)
    {
        m_mutex.unlock();
        LOG(ERROR) << "frame:" << frame->width << "x" << frame->height << ", widget:" << m_width << "x" << m_height;
        return;
    }
    if (frame->linesize[0] == m_width)  // �������
    {
        memcpy(m_yuv[0], frame->data[0], m_width * m_height);
        memcpy(m_yuv[1], frame->data[1], m_width * m_height / 4);   // ��߶���2�� yuv420P
        memcpy(m_yuv[2], frame->data[2], m_width * m_height / 4);
    }
    else
    {
        // y
        for (int i = 0; i < m_height; ++i)
        {
            memcpy(m_yuv[0] + m_width * i, frame->data[0] + frame->linesize[0] * i, m_width);
        }

        // U
        for (int i = 0; i < m_height / 2; ++i)
        {
            memcpy(m_yuv[1] + m_width / 2 * i, frame->data[1] + frame->linesize[1] * i, m_width);
        }

        // V
        for (int i = 0; i < m_height / 2; ++i)
        {
            memcpy(m_yuv[2] + m_width / 2 * i, frame->data[2] + frame->linesize[2] * i, m_width);
        }
    }
   
    m_mutex.unlock();

    update();   // ˢ����ʾ
}

#define GET_STR(x) #x   //�Զ���˫����
#define A_VER   3
#define T_VER   4

//����shader
static const char *vString = GET_STR(
    attribute vec4 vertexIn;
    attribute vec2 textureIn;
    varying vec2 textureOut;

    void main(void)
    {
        gl_Position = vertexIn;
        textureOut = textureIn;
    }
);

//ƬԪshader
static const char *tString = GET_STR(
    varying vec2 textureOut;
    uniform sampler2D tex_y;
    uniform sampler2D tex_u;
    uniform sampler2D tex_v;

    void main(void)
    {
        vec3 yuv;
        vec3 rgb;
        yuv.x = texture2D(tex_y, textureOut).r;
        yuv.y = texture2D(tex_u, textureOut).r - 0.5;
        yuv.z = texture2D(tex_v, textureOut).r - 0.5;
        rgb = mat3(1.0, 1.0, 1.0,
            0.0, -0.39465, 2.03211,
            1.13983, -0.58060, 0.0) * yuv;

        gl_FragColor = vec4(rgb, 1.0); // rgba
    }
);

void VideoWidget::initializeGL()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    LOG(INFO) << "init QOpenGl";

    // 1.��ʼ��opengl (QOpenGLFunctions�̳�)����
    initializeOpenGLFunctions();

    // 2.program����shader(�����ƬԪ)�ű�
    ///����shader
    m_shader.addShaderFromSourceCode(QGLShader::Vertex, vString);

    ///ƬԪ�����أ�
    m_shader.addShaderFromSourceCode(QGLShader::Fragment, tString);

    ///���ö�������ı���
    m_shader.bindAttributeLocation("vertexIn", A_VER);

    ///���ò�������
    m_shader.bindAttributeLocation("textureIn", T_VER);

    // 3.����shader
    LOG(INFO) << "shader link() = " << m_shader.link();
    LOG(INFO) << "shader bind() = " << m_shader.bind();

    // 4.���ݶ���Ͳ�������
    ///����
    static const GLfloat ver[] = {
        -1.0f, -1.0f,
        1.0f,  -1.0f,
        -1.0f, 1.0f,
        1.0f,  1.0f
    };

    ///��������
    static const GLfloat tex[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    ///����
    glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
    glEnableVertexAttribArray(A_VER);

    ///����
    glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
    glEnableVertexAttribArray(T_VER);

    ///��shader��ȡ����
    m_unis[0] = m_shader.uniformLocation("tex_y");
    m_unis[1] = m_shader.uniformLocation("tex_u");
    m_unis[2] = m_shader.uniformLocation("tex_v");
}

void VideoWidget::resizeGL(int w, int h)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    LOG(INFO) << "resize " << w << " x " << h;
}

void VideoWidget::paintGL()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (!(m_yuv[0] && m_yuv[1] && m_yuv[2]))
    {
        return;
    }
    // Y
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texs[0]);  //0��󶨵�Y����
    ///�޸Ĳ������ݣ������ڴ����ݣ�
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED, GL_UNSIGNED_BYTE, m_yuv[0]);
    ///��shader��������
    glUniform1i(m_unis[0], 0);

    // U
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, m_texs[1]);  //1��󶨵�U����
    ///�޸Ĳ������ݣ������ڴ����ݣ�
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width / 2, m_height / 2, GL_RED, GL_UNSIGNED_BYTE, m_yuv[1]);
    ///��shader��������
    glUniform1i(m_unis[1], 1);

    // V
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, m_texs[2]);  //2��󶨵�V����
    ///�޸Ĳ������ݣ������ڴ����ݣ�
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width / 2, m_height / 2, GL_RED, GL_UNSIGNED_BYTE, m_yuv[2]);
    ///��shader��������
    glUniform1i(m_unis[2], 2);

    // ��ʾ
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}