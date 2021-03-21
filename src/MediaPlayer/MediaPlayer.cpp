#include "MediaPlayer.h"
#include "MediaPlayThread.h"
#include "glog/logging.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QSizePolicy>
#include <QThread>

#include <QDebug>

#define RES_ICON_PLAY           "play_px48"
#define RES_ICON_PAUSE          "pause_px48"
#define RES_ICON_VOL_MUTE       "vol_mute_px48"
#define RES_ICON_VOL_LOW        "vol_low_px48"
#define RES_ICON_VOL_HIGH       "vol_high_px48"
#define RES_ICON_WIN_MAX        "win_max_px32"
#define RES_ICON_WIN_RECOVER    "win_recover_px32"
#define RES_ICON_VIDEO_FULL     "video_full_px48"
#define RES_ICON_VIDEO_RECOVER  "video_recover_px48"

#define STYLE_SHEET_ICON(PICTURE)   \
    "QPushButton{ background:transparent; }"    \
    "QPushButton::!hover{ image:url(:/normal/Resources/normal/" PICTURE "_normal.png); }" \
    "QPushButton::hover{ image:url(:/hover/Resources/hover/" PICTURE "_hover.png); }" \
    "QPushButton::pressed{ image:url(:/pressed/Resources/pressed/" PICTURE "_pressed.png); }"

#define TEXT_OPEN_FILE  "打开文件"
#define TEXT_OPEN_URL   "打开URL"

const int64_t speed_time = 1000 * 10;   // 一次快进10秒

MediaPlayer::MediaPlayer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_openUrlDialog = new OpenUrl(this);
    ui_openUrl.setupUi(m_openUrlDialog);

    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏

	setInitIcon();

    QObject::connect(ui.btn_maximize, SIGNAL(clicked()), this, SLOT(windowZoom()));
    QObject::connect(ui.btn_fullScreen, SIGNAL(clicked()), this, SLOT(videoZoom()));
    QObject::connect(ui.comboBox_open, SIGNAL(activated(const QString&)), this, SLOT(openFileOrUrl(const QString&)));

    m_media = new MediaPlayThread;
    QWidget::startTimer(100);
}

MediaPlayer::~MediaPlayer()
{
    m_media->close();
    delete m_media;
}

bool MediaPlayer::openfile(const QString& filename)
{
    if (filename.isEmpty())
    {
        LOG(ERROR) << "file name is empty!";
        return false;
    }

    if (!m_media->open(filename.toLocal8Bit(), ui.videoWidget))
    {
        QString err = "open file:" + filename + " failed!";
        QMessageBox::critical(this, "error", err);
        LOG(ERROR) << err.toStdString();
        return false;
    }
    
    m_isOpen = true;
    m_isPlay = true;
    m_filename = filename;
    ui.label_filename->setText(filename);
    ui.comboBox_open->hide();
    ui.hSlider_progress->show();
    ui.label_playtime->show();
    setPauseIcon(m_media->pause());

    m_time.restart();

    return true;
}

void MediaPlayer::setInitIcon()
{
	ui.btn_play->setStyleSheet(STYLE_SHEET_ICON(RES_ICON_PLAY));
    ui.hSlider_progress->hide();
    ui.label_playtime->hide();
    ui.label_filename->clear();
    ui.comboBox_open->show();

    ui.top_bar->setStyleSheet("background-color:rgb(20, 20, 20)");
    ui.bottom_bar->setStyleSheet("background-color:rgb(20, 20, 20)"); 
}

void MediaPlayer::setPauseIcon(bool pause)
{
    if (!m_isPlay)
    {
        return;
    }

    if (pause)
    {
        ui.btn_play->setStyleSheet(STYLE_SHEET_ICON(RES_ICON_PLAY));
    }
    else
    {
        ui.btn_play->setStyleSheet(STYLE_SHEET_ICON(RES_ICON_PAUSE));
    }
}

void MediaPlayer::fullScreen()
{
    ui.top_bar->setStyleSheet("background-color:rgba(20, 20, 20, 100)");
    ui.bottom_bar->setStyleSheet("background-color:rgba(20, 20, 20, 100)");
    ui.btn_maximize->hide();
    ui.top_bar->hide();
    ui.bottom_bar->hide();
    QRect scRect = QApplication::desktop()->screenGeometry(this);
    ui.videoWidget->resize(scRect.width(), scRect.height());
    ui.videoWidget->move(0, 0);
}

WindowStretchType MediaPlayer::getWindowStretchType(const QPoint& point)
{
    QRect rect = this->rect();
    int x = point.x();
    int y = point.y();
    QPoint topLeft = mapToGlobal(rect.topLeft());
    QPoint bottomRight = mapToGlobal(rect.bottomRight());
    int direct = STRETCH_KEEP;

    if (topLeft.y() <= y && topLeft.y() + m_borderWidth >= y)
    {
        // 上边框
        direct |= STRETCH_TOP;
    }
    if (bottomRight.y() >= y && bottomRight.y() - m_borderWidth <= y)
    {
        // 下边框
        direct |= STRETCH_BOTTOM;
    }
    if (topLeft.x() <= x && topLeft.x() + m_borderWidth >= x)
    {
        // 左边框
        direct |= STRETCH_LEFT;
    }
    if (bottomRight.x() >= x && bottomRight.x() - m_borderWidth <= x)
    {
        // 右边框
        direct |= STRETCH_RIGHT;
    }

    return (WindowStretchType)direct;
}

void MediaPlayer::updateMouseStyle(WindowStretchType direction)
{
    // 设置鼠标样式
    switch (direction)
    {
    case STRETCH_TOP:
    case STRETCH_BOTTOM:
        setCursor(Qt::SizeVerCursor);
        break;
    case STRETCH_LEFT:
    case STRETCH_RIGHT:
        setCursor(Qt::SizeHorCursor);
        break;
    case STRETCH_TOP_LEFT:
    case STRETCH_BOTTOM_RIGHT:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case STRETCH_TOP_RIGHT:
    case STRETCH_BOTTOM_LEFT:
        setCursor(Qt::SizeBDiagCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

void MediaPlayer::updateWindowSize()
{
    QRect windowRect = m_windowSizeBeforeStretch;
    int offsetX = m_endStretchPoint.x() - m_startStretchPoint.x();
    int offsetY = m_endStretchPoint.y() - m_startStretchPoint.y();

    if (STRETCH_TOP == m_stretchType)
    {
        QPoint topLeft = windowRect.topLeft();
        topLeft.setY(topLeft.y() + offsetY);
        windowRect.setTopLeft(topLeft);
    }
    else if (STRETCH_LEFT == m_stretchType)
    {
        QPoint topLeft = windowRect.topLeft();
        topLeft.setX(topLeft.x() + offsetX);
        windowRect.setTopLeft(topLeft);
    }
    else if (STRETCH_BOTTOM == m_stretchType)
    {
        QPoint bottomRight = windowRect.bottomRight();
        bottomRight.setY(bottomRight.y() + offsetY);
        windowRect.setBottomRight(bottomRight);
    }
    else if (STRETCH_RIGHT == m_stretchType)
    {
        QPoint bottomRight = windowRect.bottomRight();
        bottomRight.setX(bottomRight.x() + offsetX);
        windowRect.setBottomRight(bottomRight);
    }

    else if (STRETCH_TOP_LEFT == m_stretchType)
    {
        QPoint topLeft = windowRect.topLeft();
        topLeft.setX(topLeft.x() + offsetX);
        topLeft.setY(topLeft.y() + offsetY);
        windowRect.setTopLeft(topLeft);
    }
    else if (STRETCH_BOTTOM_LEFT == m_stretchType)
    {
        QPoint bottomLeft = windowRect.bottomLeft();
        bottomLeft.setX(bottomLeft.x() + offsetX);
        bottomLeft.setY(bottomLeft.y() + offsetY);
        windowRect.setBottomLeft(bottomLeft);
    }
    else if (STRETCH_TOP_RIGHT == m_stretchType)
    {
        QPoint topRight = windowRect.topRight();
        topRight.setX(topRight.x() + offsetX);
        topRight.setY(topRight.y() + offsetY);
        windowRect.setTopRight(topRight);
    }
    else if (STRETCH_BOTTOM_RIGHT == m_stretchType)
    {
        QPoint bottomRight = windowRect.bottomRight();
        bottomRight.setX(bottomRight.x() + offsetX);
        bottomRight.setY(bottomRight.y() + offsetY);
        windowRect.setBottomRight(bottomRight);
    }

    // 拉伸注意窗口设置的最小值
    if (windowRect.width() >= minimumSize().width() && windowRect.height() >= minimumSize().height())
    {
        setGeometry(windowRect);
    }
}

void MediaPlayer::timerEvent(QTimerEvent * event)
{
    if (!m_isPlay) { return; }

    if (isFullScreen() && m_time.elapsed() >= 5000)
    {
        ui.top_bar->hide();
        ui.bottom_bar->hide();
    }

    int64_t play_time = m_media->pts();
    int64_t file_time = m_media->totalMs();

    if (!m_sliderPressed && file_time > 0)
    {
        double pos = (double)play_time / (double)file_time;
        ui.hSlider_progress->setValue(ui.hSlider_progress->maximum() * pos);
    }
    
    if (!m_slideMoved && file_time > 0)
    {
        int play_time_s = play_time / 1000;
        int file_time_s = file_time / 1000;

        int play_h = play_time_s / 3600;
        int play_m = (play_time_s % 3600) / 60;
        int play_s = play_time_s % 60;

        int h = file_time_s / 3600;
        int m = (file_time_s % 3600) / 60;
        int s = file_time_s % 60;

        char buf[32] = { 0 };
        sprintf(buf, "%02d:%02d:%02d / %02d:%02d:%02d", play_h, play_m, play_s, h, m, s);
        ui.label_playtime->setText(buf);
    }

    if (file_time > 0 && file_time - play_time < 500)
    {
        LOG(INFO) << "end of file, stop!";
        stop();
    }
}

void MediaPlayer::resizeEvent(QResizeEvent * event)
{
    // 顶部只设置宽度，高度不变
    ui.top_bar->resize(width(), ui.top_bar->height());

    // 将label_filename置中
    ui.label_filename->resize(width() - ui.widget_windows->width() * 2 - 40 , ui.top_bar->height());
    ui.label_filename->move((width() - ui.label_filename->width()) / 2, ui.label_filename->y());
    ui.widget_windows->move(width() - ui.widget_windows->width(), ui.widget_windows->y());

    // 底部栏和进度条只设置宽度，高度不变
    ui.bottom_bar->resize(width(), ui.bottom_bar->height());
    ui.hSlider_progress->resize(width(), ui.hSlider_progress->height());

    // 底部的control栏置中
    ui.widget_control->move((ui.bottom_bar->width() - ui.widget_control->width()) / 2, ui.widget_control->y());
    ui.widget_tool->move(width() - ui.widget_tool->width(), ui.widget_tool->y());

    // 移动底部栏 comboBox位置
    ui.bottom_bar->move(0, height() - ui.bottom_bar->height());
    ui.comboBox_open->move((width() - ui.comboBox_open->width()) / 2, height() * 0.6);

    // 保持预先设置的视频显示比例尺不变
    QRect scRect = QApplication::desktop()->screenGeometry(this);  // 获取UI当前窗口
    QSize size = ui.videoWidget->size();
    int w = this->size().width();
    int h = this->size().height() - ui.top_bar->height() - ui.bottom_bar->height();
    QSize widgetSize(w, h);
    double baseRate = (double)scRect.width() / (double)scRect.height();
    if (m_isOpen)
    {
        // 文件已经打开，取视频自己的宽高比
        int w = m_media->width();
        int h = m_media->height();
        if (w > 0 && h > 0)
        {
            baseRate = double(w) / (double)h;
        }
    }
    double widgetRate = (double)widgetSize.width() / (double)widgetSize.height();
    double frameRate = (double)size.width() / (double)size.height();
    if (widgetRate > baseRate)
    {
        size.setWidth(widgetSize.height() * baseRate);
        size.setHeight(widgetSize.height());
    }
    else
    {
        size.setWidth(widgetSize.width());
        size.setHeight(widgetSize.width() / baseRate);
    }
    ui.videoWidget->move((w - size.width()) / 2, (h - size.height()) / 2 + ui.top_bar->height());
    ui.videoWidget->resize(size);
}

void MediaPlayer::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ui.btn_fullScreen->clicked();
    }

    QWidget::mouseDoubleClickEvent(event);
}

void MediaPlayer::mousePressEvent(QMouseEvent * event)
{
    // globalPos()获取鼠标在根窗口的位置
    // frameGeometry().topLeft()获取程序主左上角在根窗口的位置
    if (event->button() == Qt::LeftButton)
    {
        m_drag = true;
        m_dragPoint = event->globalPos() - frameGeometry().topLeft();

        // 窗口尺寸拖拽
        m_startStretchPoint = event->globalPos();
        m_windowSizeBeforeStretch = geometry();
    }

    QWidget::mousePressEvent(event);
}

void MediaPlayer::mouseReleaseEvent(QMouseEvent * event)
{
    m_drag = false;
    m_stretch = false;

    QWidget::mouseReleaseEvent(event);
}

void MediaPlayer::mouseMoveEvent(QMouseEvent * event)
{
    // 窗口移动
    if (m_drag && !m_stretch && (event->buttons() & Qt::LeftButton))
    {
        if (isMaximized())
        {
            // 窗口最大化状态下，拖动恢复原有大小
            m_drag = false;
            emit ui.btn_maximize->clicked();
        }
        else if (isFullScreen())
        {
            // 全屏状态下
        }
        else
        {
            // 移动了两次鼠标之间的差值距离
            QWidget::move(event->globalPos() - m_dragPoint);
        }
    }
    
    // 尺寸变化
    // 如果左键没有按下
    if (!(event->buttons() & Qt::LeftButton))
    {
        // 窗口不是最大化或全屏状态才可以缩放
        if (!isMaximized() && !isFullScreen())
        {
            // 根据当前鼠标位置设置样式
            QPoint mousePos = event->globalPos();
            m_stretchType = getWindowStretchType(mousePos);
            updateMouseStyle(m_stretchType);
            m_stretch = false;
            if (STRETCH_KEEP != m_stretchType)
            {
                m_stretch = true;
            }
        }
    }
    else if (m_stretch)
    {
        // 左键已经按下记录第二个点的位置，并更新窗口大小
        m_endStretchPoint = event->globalPos();
        updateWindowSize();
    }

    if (m_isPlay)
    {
        m_time.restart();
        ui.top_bar->show();
        ui.bottom_bar->show();
    }
    
    QWidget::mouseMoveEvent(event);
}

void MediaPlayer::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Left && m_isPlay)
    {
        double pos = getkeyPressSeekPos(Qt::Key_Left);
        m_media->seek(pos);
        ui.hSlider_progress->setValue(ui.hSlider_progress->maximum() * pos);
    }
    else if (event->key() == Qt::Key_Right && m_isPlay)
    {
        double pos = getkeyPressSeekPos(Qt::Key_Right);
        m_media->seek(pos);
        ui.hSlider_progress->setValue(ui.hSlider_progress->maximum() * pos);
    }
    else if (event->key() == Qt::Key_Up)
    {
    }
    else if (event->key() == Qt::Key_Down)
    {
    }
    else if (event->key() == Qt::Key_Space && m_isPlay)
    {
        playOrPause();
    }

    QWidget::keyPressEvent(event);
}

void MediaPlayer::keyReleaseEvent(QKeyEvent * event)
{
    QWidget::keyReleaseEvent(event);
}

double MediaPlayer::getkeyPressSeekPos(Qt::Key key)
{
    int64_t play_time = m_media->pts();
    int64_t file_time = m_media->totalMs();
    double pos = 0;
    if (file_time > speed_time * 20)
    {
        // 总时长大于20倍的快进时长 取soeed_time为一个快进间隔
        if (key == Qt::Key_Left) 
        {
            pos = double(play_time - speed_time) / file_time;
        }
        else if (key == Qt::Key_Right)
        {
            pos = double(play_time + speed_time) / file_time;
        }
        
    }
    else
    {
        if (key == Qt::Key_Left)
        {
            pos = double(ui.hSlider_progress->value() - ui.hSlider_progress->pageStep()) / ui.hSlider_progress->maximum();
        }
        else if (key == Qt::Key_Right)
        {
            pos = double(ui.hSlider_progress->value() + ui.hSlider_progress->pageStep()) / ui.hSlider_progress->maximum();
        }
    }
    return pos;
}

void MediaPlayer::openFileOrUrl(const QString& text)
{
    QString filename;
    if (text == QStringLiteral(TEXT_OPEN_FILE))
    {
        LOG(INFO) << TEXT_OPEN_FILE;
        filename = QFileDialog::getOpenFileName(this, QStringLiteral("选择视频文件"));
    }
    else if (text == QStringLiteral(TEXT_OPEN_URL))
    {
        LOG(INFO) << TEXT_OPEN_URL;
        m_openUrlDialog->show();
        return;
    }
    else
    {
        LOG(INFO) << "未知选项";
        return;
    }

    if (!openfile(filename))
    {
        LOG(ERROR) << "open " << filename.toLocal8Bit().constData() << " failed!";
        return;
    }
    resizeEvent(nullptr);
}

void MediaPlayer::openUrl()
{
    QString filename = ui_openUrl.comboBox_url->currentText();
    if (filename.compare("") != 0 && ui_openUrl.comboBox_url->findText(filename) == -1)
    {
        ui_openUrl.comboBox_url->insertItem(0, filename);
    }

    if (!openfile(filename))
    {
        LOG(ERROR) << "open " << filename.toLocal8Bit().constData() << " failed!";
        return;
    }
    ui.hSlider_progress->hide();
}

void MediaPlayer::playOrPause()
{
    bool status = !m_media->pause();
    m_media->setPause(status);
    setPauseIcon(status);
}

void MediaPlayer::stop()
{
    m_media->close();
    setInitIcon();
    m_isOpen = false;
    m_isPlay = false;
}

void MediaPlayer::sliderPressed()
{
    m_sliderPressed = true;
    double pos = (double)ui.hSlider_progress->value() / ui.hSlider_progress->maximum();
    m_media->seek(pos);
}

void MediaPlayer::sliderReleased()
{
    m_sliderPressed = false;
}

void MediaPlayer::sliderMoved()
{
    m_slideMoved = true;
    double pos = (double)ui.hSlider_progress->value() / ui.hSlider_progress->maximum();
    m_media->seek(pos);
    m_slideMoved = false;
    m_time.restart();
}

void MediaPlayer::clearUrlData()
{
    ui_openUrl.comboBox_url->clear();
}

void MediaPlayer::windowZoom()
{
    if (isMaximized())
    {
        showNormal();
        ui.btn_maximize->setStyleSheet(STYLE_SHEET_ICON(RES_ICON_WIN_MAX));
    }
    else
    {
        showMaximized();
        ui.btn_maximize->setStyleSheet(STYLE_SHEET_ICON(RES_ICON_WIN_RECOVER));
    }
}

void MediaPlayer::videoZoom()
{
    if (!m_isOpen)
    {
        return;
    }

    if (isFullScreen())
    {
        showNormal();
        ui.btn_fullScreen->setStyleSheet(STYLE_SHEET_ICON(RES_ICON_VIDEO_FULL));
        ui.btn_maximize->setStyleSheet(STYLE_SHEET_ICON(RES_ICON_WIN_MAX));
        ui.top_bar->setStyleSheet("background-color:rgb(20, 20, 20)");
        ui.bottom_bar->setStyleSheet("background-color:rgb(20, 20, 20)");
        ui.btn_maximize->show();
        ui.top_bar->show();
        ui.bottom_bar->show();
    }
    else
    {
        showFullScreen();
        ui.btn_fullScreen->setStyleSheet(STYLE_SHEET_ICON(RES_ICON_VIDEO_RECOVER));
        fullScreen();
    }
}
