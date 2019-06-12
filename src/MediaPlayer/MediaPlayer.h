#pragma once

#include <QtWidgets/QWidget>
#include <QTime>
#include "ui_MediaPlayer.h"
#include "OpenUrl.h"

class MediaPlayThread;

class MediaPlayer :
    public QWidget
{
    Q_OBJECT

public:
    MediaPlayer(QWidget *parent = Q_NULLPTR);
    ~MediaPlayer();

    bool open(QString filename);
    void setPauseIcon(bool pause);
protected:
    virtual void timerEvent(QTimerEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    // Ë«»÷Êó±êÈ«ÆÁ
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

public slots:
    void openFileOrUrl();
    void openUrl();
    void playOrPause();
    void stop();

    void sliderPressed();
    void sliderReleased();
    void sliderMoved();
    void clearUrlData();
private:
    Ui::MediaPlayerClass ui;
    Ui::DialogOpenUrl ui_openUrl;
    OpenUrl         *m_openUrlDialog = nullptr;
    MediaPlayThread *m_media = nullptr;
    QString         m_filename;
    bool            m_isPlay = false;
    bool            m_sliderPressed = false;
    bool            m_slideMoved = false;
    bool            m_move = false;
    QPoint          m_dragPoint;
    QTime           m_time;
    QTime           m_volume_time;
};
