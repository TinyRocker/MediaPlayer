#include "MediaPlayer.h"
#include "MediaPlayThread.h"
#include "glog/logging.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QThread>

MediaPlayer::MediaPlayer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_openUrlDialog = new OpenUrl(this);
    ui_openUrl.setupUi(m_openUrlDialog);

    ui.pushButton_play->setStyleSheet("border-image: url(:/MediaPlayer/Resources/play_32px.ico)");
    ui.pushButton_sound->setStyleSheet("border-image: url(:/MediaPlayer/Resources/speaker_24px.ico)");
    ui.horizontalSlider_media->hide();
    ui.label_playtime->hide();
    ui.bottom_bar->setMouseTracking(true);
    ui.videoWidget->setMouseTracking(true);

    m_media = new MediaPlayThread;
    QWidget::startTimer(50);
}

MediaPlayer::~MediaPlayer()
{
    m_media->close();
    delete m_media;
}

bool MediaPlayer::open(QString filename)
{
    if (filename.isEmpty())
    {
        LOG(ERROR) << "file name is empty!";
        return false;
    }
    if (!m_media->open(filename.toLocal8Bit(), ui.videoWidget))
    {
        QString err = "open file:" + filename + " failed!";
        QMessageBox::information(0, "error", err);
        LOG(ERROR) << err.toStdString();
        return false;
    }
    
    m_isPlay = true;
    m_filename = filename;
    this->setWindowTitle(filename);
    ui.comboBox_open->hide();
    ui.horizontalSlider_media->show();
    ui.label_playtime->show();
    setPauseIcon(m_media->pause());
    m_time.restart();

    return true;
}

void MediaPlayer::setPauseIcon(bool pause)
{
    if (!m_isPlay) { return; }
    if (pause) ui.pushButton_play->setStyleSheet("border-image: url(:/MediaPlayer/Resources/play_32px.ico)");
    else ui.pushButton_play->setStyleSheet("border-image: url(:/MediaPlayer/Resources/pause_32px.ico)");
}

void MediaPlayer::timerEvent(QTimerEvent * event)
{
    if (!m_isPlay) { return; }

    if (m_time.elapsed() >= 3000)
    {
        ui.bottom_bar->hide();
    }

    int64_t play_time = m_media->pts();
    int64_t file_time = m_media->totalMs();

    if (!m_sliderPressed && file_time > 0)
    {
        double pos = (double)play_time / (double)file_time;
        ui.horizontalSlider_media->setValue(ui.horizontalSlider_media->maximum() * pos);
    }
    
    if (!m_slideMoved && file_time >= 0)
    {
        play_time /= 1000;
        file_time /= 1000;

        int play_h = play_time / 3600;
        int play_m = (play_time % 3600) / 60;
        int play_s = play_time % 60;

        int h = file_time / 3600;
        int m = (file_time % 3600) / 60;
        int s = file_time % 60;

        char buf[32] = { 0 };
        sprintf(buf, "%02d:%02d:%02d / %02d:%02d:%02d", play_h, play_m, play_s, h, m, s);
        ui.label_playtime->setText(buf);
    }
}

void MediaPlayer::resizeEvent(QResizeEvent * event)
{
    ui.videoWidget->resize(this->size());
    ui.bottom_bar->resize(this->width(), ui.bottom_bar->height());
    ui.horizontalSlider_media->resize(ui.bottom_bar->width(), ui.horizontalSlider_media->height());
    ui.control_bar->move((ui.bottom_bar->width()-ui.control_bar->width()) / 2, ui.control_bar->y());
    ui.bottom_bar->move(0, this->height() - ui.bottom_bar->height());
    ui.comboBox_open->move((this->width()-ui.comboBox_open->width()) / 2, this->height() * 0.618);
}

void MediaPlayer::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (isFullScreen())
    {
        this->showNormal();
    }
    else
    {
        this->showFullScreen();
    }
}

void MediaPlayer::mousePressEvent(QMouseEvent * event)
{
    // globalPos()获取鼠标在根窗口的位置
    // frameGeometry().topLeft()获取程序主左上角在根窗口的位置
    m_move = false;
    if (event->button() == Qt::LeftButton)
    {
        m_dragPoint = event->globalPos() - frameGeometry().topLeft();
        m_move = true;
        event->accept();
    }
}

void MediaPlayer::mouseReleaseEvent(QMouseEvent * event)
{
    m_move = false;
    event->accept();
}

void MediaPlayer::mouseMoveEvent(QMouseEvent * event)
{
    // 移动了两次鼠标之间的差值距离
    if (m_move && event->buttons() == Qt::LeftButton)
    {
        QWidget::move(event->globalPos() - m_dragPoint);
        event->accept();
    }
    else if (m_isPlay)
    {
        m_time.restart();
        ui.bottom_bar->show();
    }
}

void MediaPlayer::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Left && m_isPlay)
    {
        m_sliderPressed = true;
        ui.horizontalSlider_media->setValue(ui.horizontalSlider_media->value() - ui.horizontalSlider_media->singleStep());
        double pos = (double)ui.horizontalSlider_media->value() / ui.horizontalSlider_media->maximum();
        m_media->seek(pos);
        m_sliderPressed = false;
    }
    else if (event->key() == Qt::Key_Right && m_isPlay)
    {
        m_sliderPressed = true;
        ui.horizontalSlider_media->setValue(ui.horizontalSlider_media->value() + ui.horizontalSlider_media->singleStep());
        double pos = (double)ui.horizontalSlider_media->value() / ui.horizontalSlider_media->maximum();
        m_media->seek(pos);
        m_sliderPressed = false;
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
    event->accept();
}

void MediaPlayer::keyReleaseEvent(QKeyEvent * event)
{
}

void MediaPlayer::openFileOrUrl()
{
    QString filename;
    if (ui.comboBox_open->currentText().toLocal8Bit().compare("打开文件") == 0)
    {
        LOG(INFO) << "打开文件";
        filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频文件"));
    }
    else if (ui.comboBox_open->currentText().toLocal8Bit().compare("打开url") == 0)
    {
        LOG(INFO) << "打开url";
        m_openUrlDialog->show();
        return;
    }
    else
    {
        LOG(INFO) << "未知选项";
        return;
    }

    if (!open(filename))
    {
        LOG(ERROR) << "open " << filename.toLocal8Bit().constData() << " failed!";
        return;
    }
}

void MediaPlayer::openUrl()
{
    //const char *liveurl = "rtmp://live.hkstv.hk.lxdns.com/live/hks2"; // 香港卫视
    QString filename = ui_openUrl.comboBox_url->currentText();
    if (!open(filename))
    {
        LOG(ERROR) << "open " << filename.toLocal8Bit().constData() << " failed!";
        return;
    }
    ui.horizontalSlider_media->hide();
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
    ui.pushButton_play->setStyleSheet("border-image: url(:/MediaPlayer/Resources/play_32px.ico)");
    ui.comboBox_open->show();
    ui.horizontalSlider_media->hide();
    ui.label_playtime->setText("00:00:00 / 00:00:00");
    ui.label_playtime->hide();
    this->setWindowTitle("MediaPlayer");
    m_isPlay = false;
}

void MediaPlayer::sliderPressed()
{
    m_sliderPressed = true;
    double pos = (double)ui.horizontalSlider_media->value() / ui.horizontalSlider_media->maximum();
    m_media->seek(pos);
}

void MediaPlayer::sliderReleased()
{
    m_sliderPressed = false;
}

void MediaPlayer::sliderMoved()
{
    m_slideMoved = true;
    double pos = (double)ui.horizontalSlider_media->value() / ui.horizontalSlider_media->maximum();
    m_media->seek(pos);
    m_slideMoved = false;
    m_time.restart();
}