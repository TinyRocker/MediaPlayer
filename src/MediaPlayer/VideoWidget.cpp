#include "VideoWidget.h"
#include "glog/logging.h"

extern "C"
{
#include "libavutil/frame.h"
}

//准备yuv数据
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

    //分配材质内存空间 yuv420
    m_yuv[0] = new uint8_t[m_width * m_height];       ///Y
    m_yuv[1] = new uint8_t[m_width * m_height / 4];   ///U
    m_yuv[2] = new uint8_t[m_width * m_height / 4];   ///V
    if (!m_yuv[0] || !m_yuv[1] || !m_yuv[2])
    {
        LOG(ERROR) << "malloc memory failed!";
        return false;
    }

    // 5.创建材质
    glGenTextures(3, m_texs);

    //Y
    glBindTexture(GL_TEXTURE_2D, m_texs[0]);
    ///GL_NEAREST(临近差值，效率高，但马赛克严重) 
    ///GL_LINEAR (线性插值，效率低，画面质量好)
    ///放大过滤，线性插值  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ///缩小过滤，线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ///创建材质空间
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //U
    glBindTexture(GL_TEXTURE_2D, m_texs[1]);
    ///放大过滤， 线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ///缩小过滤， 线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ///创建材质空间
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width / 2, m_height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //V
    glBindTexture(GL_TEXTURE_2D, m_texs[2]);
    ///放大过滤， 线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ///缩小过滤， 线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ///创建材质显卡空间
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

    // 行对齐问题
    m_mutex.lock();
    if (frame->width != m_width || frame->height != m_height)
    {
        m_mutex.unlock();
        LOG(ERROR) << "frame:" << frame->width << "x" << frame->height << ", widget:" << m_width << "x" << m_height;
        return;
    }
    if (frame->linesize[0] == m_width)  // 无需对齐
    {
        memcpy(m_yuv[0], frame->data[0], m_width * m_height);
        memcpy(m_yuv[1], frame->data[1], m_width * m_height / 4);   // 宽高都除2， yuv420P
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

    update();   // 刷新显示
}

#define GET_STR(x) #x   //自动加双引号
#define A_VER   3
#define T_VER   4

//顶点shader
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

//片元shader
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

    // 1.初始化opengl (QOpenGLFunctions继承)函数
    initializeOpenGLFunctions();

    // 2.program加载shader(顶点和片元)脚本
    ///顶点shader
    m_shader.addShaderFromSourceCode(QGLShader::Vertex, vString);

    ///片元（像素）
    m_shader.addShaderFromSourceCode(QGLShader::Fragment, tString);

    ///设置顶点坐标的变量
    m_shader.bindAttributeLocation("vertexIn", A_VER);

    ///设置材质坐标
    m_shader.bindAttributeLocation("textureIn", T_VER);

    // 3.编译shader
    LOG(INFO) << "shader link() = " << m_shader.link();
    LOG(INFO) << "shader bind() = " << m_shader.bind();

    // 4.传递顶点和材质坐标
    ///顶点
    static const GLfloat ver[] = {
        -1.0f, -1.0f,
        1.0f,  -1.0f,
        -1.0f, 1.0f,
        1.0f,  1.0f
    };

    ///材质坐标
    static const GLfloat tex[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    ///顶点
    glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
    glEnableVertexAttribArray(A_VER);

    ///材质
    glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
    glEnableVertexAttribArray(T_VER);

    ///从shader获取材质
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
    glBindTexture(GL_TEXTURE_2D, m_texs[0]);  //0层绑定到Y材质
    ///修改材质内容（复制内存内容）
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED, GL_UNSIGNED_BYTE, m_yuv[0]);
    ///与shader变量关联
    glUniform1i(m_unis[0], 0);

    // U
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, m_texs[1]);  //1层绑定到U材质
    ///修改材质内容（复制内存内容）
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width / 2, m_height / 2, GL_RED, GL_UNSIGNED_BYTE, m_yuv[1]);
    ///与shader变量关联
    glUniform1i(m_unis[1], 1);

    // V
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, m_texs[2]);  //2层绑定到V材质
    ///修改材质内容（复制内存内容）
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width / 2, m_height / 2, GL_RED, GL_UNSIGNED_BYTE, m_yuv[2]);
    ///与shader变量关联
    glUniform1i(m_unis[2], 2);

    // 显示
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}