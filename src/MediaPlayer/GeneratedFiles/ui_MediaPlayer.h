/********************************************************************************
** Form generated from reading UI file 'MediaPlayer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDIAPLAYER_H
#define UI_MEDIAPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "Slider.h"
#include "VideoWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MediaPlayerClass
{
public:
    VideoWidget *videoWidget;
    QWidget *bottom_bar;
    QWidget *control_bar;
    QPushButton *pushButton_stop;
    QPushButton *pushButton_previous;
    QPushButton *pushButton_play;
    QPushButton *pushButton_next;
    QPushButton *pushButton_sound;
    Slider *horizontalSlider_media;
    QLabel *label_playtime;
    QComboBox *comboBox_open;

    void setupUi(QWidget *MediaPlayerClass)
    {
        if (MediaPlayerClass->objectName().isEmpty())
            MediaPlayerClass->setObjectName(QString::fromUtf8("MediaPlayerClass"));
        MediaPlayerClass->resize(800, 500);
        MediaPlayerClass->setMouseTracking(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/MediaPlayer/Resources/MediaPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MediaPlayerClass->setWindowIcon(icon);
        MediaPlayerClass->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        videoWidget = new VideoWidget(MediaPlayerClass);
        videoWidget->setObjectName(QString::fromUtf8("videoWidget"));
        videoWidget->setGeometry(QRect(0, 0, 800, 500));
        videoWidget->setFocusPolicy(Qt::StrongFocus);
        videoWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        bottom_bar = new QWidget(MediaPlayerClass);
        bottom_bar->setObjectName(QString::fromUtf8("bottom_bar"));
        bottom_bar->setGeometry(QRect(0, 440, 800, 60));
        bottom_bar->setMouseTracking(false);
        bottom_bar->setStyleSheet(QString::fromUtf8("background-color: rgba(38, 76, 115, 150);"));
        control_bar = new QWidget(bottom_bar);
        control_bar->setObjectName(QString::fromUtf8("control_bar"));
        control_bar->setGeometry(QRect(300, 20, 200, 32));
        control_bar->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        pushButton_stop = new QPushButton(control_bar);
        pushButton_stop->setObjectName(QString::fromUtf8("pushButton_stop"));
        pushButton_stop->setGeometry(QRect(8, 4, 24, 24));
        pushButton_stop->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/MediaPlayer/Resources/stop_24px.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_stop->setIcon(icon1);
        pushButton_stop->setIconSize(QSize(24, 24));
        pushButton_stop->setFlat(true);
        pushButton_previous = new QPushButton(control_bar);
        pushButton_previous->setObjectName(QString::fromUtf8("pushButton_previous"));
        pushButton_previous->setGeometry(QRect(46, 4, 24, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/MediaPlayer/Resources/previous_24px.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_previous->setIcon(icon2);
        pushButton_previous->setIconSize(QSize(24, 24));
        pushButton_previous->setFlat(true);
        pushButton_play = new QPushButton(control_bar);
        pushButton_play->setObjectName(QString::fromUtf8("pushButton_play"));
        pushButton_play->setEnabled(true);
        pushButton_play->setGeometry(QRect(84, 0, 32, 32));
        pushButton_play->setIconSize(QSize(32, 32));
        pushButton_play->setFlat(false);
        pushButton_next = new QPushButton(control_bar);
        pushButton_next->setObjectName(QString::fromUtf8("pushButton_next"));
        pushButton_next->setGeometry(QRect(130, 4, 24, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/MediaPlayer/Resources/next_24px.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_next->setIcon(icon3);
        pushButton_next->setIconSize(QSize(24, 24));
        pushButton_next->setFlat(true);
        pushButton_sound = new QPushButton(control_bar);
        pushButton_sound->setObjectName(QString::fromUtf8("pushButton_sound"));
        pushButton_sound->setGeometry(QRect(168, 4, 24, 24));
        pushButton_sound->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        pushButton_sound->setIconSize(QSize(24, 24));
        pushButton_sound->setFlat(false);
        horizontalSlider_media = new Slider(bottom_bar);
        horizontalSlider_media->setObjectName(QString::fromUtf8("horizontalSlider_media"));
        horizontalSlider_media->setGeometry(QRect(0, 0, 800, 15));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(horizontalSlider_media->sizePolicy().hasHeightForWidth());
        horizontalSlider_media->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(38, 76, 115, 150));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        horizontalSlider_media->setPalette(palette);
        QFont font;
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        horizontalSlider_media->setFont(font);
        horizontalSlider_media->setFocusPolicy(Qt::StrongFocus);
        horizontalSlider_media->setContextMenuPolicy(Qt::DefaultContextMenu);
        horizontalSlider_media->setMaximum(999);
        horizontalSlider_media->setPageStep(1);
        horizontalSlider_media->setValue(0);
        horizontalSlider_media->setOrientation(Qt::Horizontal);
        horizontalSlider_media->setInvertedAppearance(false);
        horizontalSlider_media->setInvertedControls(false);
        horizontalSlider_media->setTickPosition(QSlider::NoTicks);
        horizontalSlider_media->setTickInterval(0);
        label_playtime = new QLabel(bottom_bar);
        label_playtime->setObjectName(QString::fromUtf8("label_playtime"));
        label_playtime->setGeometry(QRect(10, 25, 130, 20));
        label_playtime->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(0, 0, 0, 0);\n"
"font: 11pt \"Arial\";"));
        comboBox_open = new QComboBox(MediaPlayerClass);
        comboBox_open->addItem(QString());
        comboBox_open->addItem(QString());
        comboBox_open->setObjectName(QString::fromUtf8("comboBox_open"));
        comboBox_open->setGeometry(QRect(350, 300, 100, 30));
        comboBox_open->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(38, 76, 115, 150);\n"
"font: 11pt \"Arial\";"));

        retranslateUi(MediaPlayerClass);
        QObject::connect(pushButton_play, SIGNAL(clicked()), MediaPlayerClass, SLOT(playOrPause()));
        QObject::connect(pushButton_stop, SIGNAL(clicked()), MediaPlayerClass, SLOT(stop()));
        QObject::connect(comboBox_open, SIGNAL(activated(QString)), MediaPlayerClass, SLOT(openFileOrUrl()));
        QObject::connect(horizontalSlider_media, SIGNAL(sliderPressed()), MediaPlayerClass, SLOT(sliderPressed()));
        QObject::connect(horizontalSlider_media, SIGNAL(sliderReleased()), MediaPlayerClass, SLOT(sliderReleased()));
        QObject::connect(horizontalSlider_media, SIGNAL(sliderMoved(int)), MediaPlayerClass, SLOT(sliderMoved()));

        QMetaObject::connectSlotsByName(MediaPlayerClass);
    } // setupUi

    void retranslateUi(QWidget *MediaPlayerClass)
    {
        MediaPlayerClass->setWindowTitle(QApplication::translate("MediaPlayerClass", "MediaPlayer", nullptr));
        pushButton_stop->setText(QString());
        pushButton_previous->setText(QString());
        pushButton_play->setText(QString());
        pushButton_next->setText(QString());
        pushButton_sound->setText(QString());
        label_playtime->setText(QApplication::translate("MediaPlayerClass", "00:00:00 / 00:00:00", nullptr));
        comboBox_open->setItemText(0, QApplication::translate("MediaPlayerClass", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
        comboBox_open->setItemText(1, QApplication::translate("MediaPlayerClass", "\346\211\223\345\274\200url", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MediaPlayerClass: public Ui_MediaPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIAPLAYER_H
