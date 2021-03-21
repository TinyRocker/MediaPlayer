#pragma onceSTRETCH_LEFT

#include <QtWidgets/QWidget>
#include <QTime>
#include "ui_MediaPlayer.h"
#include "OpenUrl.h"

class MediaPlayThread;

enum WindowStretchType
{
    STRETCH_KEEP = 0x0000,             // 保持不动
    STRETCH_TOP = 0x0001,              // 上
    STRETCH_LEFT = 0x0010,             // 左
    STRETCH_BOTTOM = 0x0100,           // 下
    STRETCH_RIGHT = 0x1000,            // 右

    STRETCH_TOP_LEFT = 0x0011,         // 左上
    STRETCH_BOTTOM_LEFT = 0x0110,      // 左下
    STRETCH_TOP_RIGHT = 0x1001,        // 右上
    STRETCH_BOTTOM_RIGHT = 0x1100      // 右下  
};

class MediaPlayer :
    public QWidget
{
    Q_OBJECT

public:
    MediaPlayer(QWidget *parent = Q_NULLPTR);
    ~MediaPlayer();

    bool openfile(const QString& filename);
	void setInitIcon();
	void setPauseIcon(bool pause);
    void fullScreen();
    WindowStretchType getWindowStretchType(const QPoint& cursorGlobalPoint);
    void updateMouseStyle(WindowStretchType direction);
    void updateWindowSize();

protected:
    virtual void timerEvent(QTimerEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    // 双击鼠标全屏
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    double getkeyPressSeekPos(Qt::Key key);

private slots:
    void openFileOrUrl(const QString& text);
    void openUrl();
    void playOrPause();
    void stop();

    void sliderPressed();
    void sliderReleased();
    void sliderMoved();
    void clearUrlData();
    void windowZoom();
    void videoZoom();

private:
    Ui::MediaPlayerClass ui;
    Ui::DialogOpenUrl ui_openUrl;
    OpenUrl         *m_openUrlDialog = nullptr;
    MediaPlayThread *m_media = nullptr;

    QString         m_filename;
    bool            m_isOpen = false;
    bool            m_isPlay = false;
    bool            m_sliderPressed = false;
    bool            m_slideMoved = false;
    bool            m_videoFull = false;
    
    QTime           m_time;
    QTime           m_volume_time;

    // 窗口拖拽
    bool            m_drag = false;
    QPoint          m_dragPoint;

    // 窗口边框拖拽
    bool            m_stretch = false;         // 当前窗口是否处于可拖拽窗口大小状态
    int             m_borderWidth = 6;         // 窗口边缘像素数，用于拖动变换窗口大小
    QPoint          m_startStretchPoint;       // 拖拽开始的位置
    QPoint          m_endStretchPoint;         // 拖曳结束的位置
    QRect           m_windowSizeBeforeStretch; // 拖曳之前的窗口大小
    WindowStretchType  m_stretchType;          // 拖动方向
};
