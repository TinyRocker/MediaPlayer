/********************************************************************************
** Form generated from reading UI file 'MediaPlayer.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDIAPLAYER_H
#define UI_MEDIAPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "Slider.h"
#include "VideoWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MediaPlayerClass
{
public:
    QWidget *top_bar;
    QLabel *label_logo;
    QLabel *label_filename;
    QWidget *widget_windows;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_minimize;
    QPushButton *btn_maximize;
    QPushButton *btn_close;
    VideoWidget *videoWidget;
    QWidget *bottom_bar;
    Slider *hSlider_progress;
    QWidget *widget_control;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_stop;
    QPushButton *btn_previous;
    QPushButton *btn_play;
    QPushButton *btn_next;
    QPushButton *btn_sound;
    QWidget *widget_tool;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_recordList;
    QPushButton *btn_fullScreen;
    QLabel *label_playtime;
    QComboBox *comboBox_open;

    void setupUi(QWidget *MediaPlayerClass)
    {
        if (MediaPlayerClass->objectName().isEmpty())
            MediaPlayerClass->setObjectName(QString::fromUtf8("MediaPlayerClass"));
        MediaPlayerClass->resize(800, 500);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MediaPlayerClass->sizePolicy().hasHeightForWidth());
        MediaPlayerClass->setSizePolicy(sizePolicy);
        MediaPlayerClass->setMinimumSize(QSize(800, 500));
        MediaPlayerClass->setMouseTracking(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/MediaPlayer/Resources/MediaPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MediaPlayerClass->setWindowIcon(icon);
        MediaPlayerClass->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 0);"));
        top_bar = new QWidget(MediaPlayerClass);
        top_bar->setObjectName(QString::fromUtf8("top_bar"));
        top_bar->setEnabled(true);
        top_bar->setGeometry(QRect(0, 0, 800, 40));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(top_bar->sizePolicy().hasHeightForWidth());
        top_bar->setSizePolicy(sizePolicy1);
        top_bar->setMinimumSize(QSize(800, 40));
        top_bar->setMaximumSize(QSize(16777215, 40));
        top_bar->setMouseTracking(true);
        top_bar->setStyleSheet(QString::fromUtf8("background-color: rgb(20, 20, 20);\n"
""));
        label_logo = new QLabel(top_bar);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));
        label_logo->setGeometry(QRect(10, 3, 36, 36));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_logo->sizePolicy().hasHeightForWidth());
        label_logo->setSizePolicy(sizePolicy2);
        label_logo->setMinimumSize(QSize(36, 36));
        label_logo->setMaximumSize(QSize(36, 36));
        label_logo->setMouseTracking(true);
        label_logo->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 0);"));
        label_logo->setTextFormat(Qt::AutoText);
        label_logo->setPixmap(QPixmap(QString::fromUtf8(":/MediaPlayer/Resources/MediaPlayer.ico")));
        label_logo->setScaledContents(true);
        label_filename = new QLabel(top_bar);
        label_filename->setObjectName(QString::fromUtf8("label_filename"));
        label_filename->setGeometry(QRect(97, 5, 571, 32));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_filename->sizePolicy().hasHeightForWidth());
        label_filename->setSizePolicy(sizePolicy3);
        label_filename->setMinimumSize(QSize(0, 32));
        label_filename->setMouseTracking(true);
        label_filename->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 0);\n"
"color: rgb(255, 255, 255);\n"
"font: 10pt;"));
        label_filename->setAlignment(Qt::AlignCenter);
        widget_windows = new QWidget(top_bar);
        widget_windows->setObjectName(QString::fromUtf8("widget_windows"));
        widget_windows->setGeometry(QRect(679, 0, 121, 40));
        sizePolicy2.setHeightForWidth(widget_windows->sizePolicy().hasHeightForWidth());
        widget_windows->setSizePolicy(sizePolicy2);
        widget_windows->setMouseTracking(true);
        widget_windows->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 0);"));
        horizontalLayout_2 = new QHBoxLayout(widget_windows);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 9, 0);
        btn_minimize = new QPushButton(widget_windows);
        btn_minimize->setObjectName(QString::fromUtf8("btn_minimize"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(24);
        sizePolicy4.setVerticalStretch(24);
        sizePolicy4.setHeightForWidth(btn_minimize->sizePolicy().hasHeightForWidth());
        btn_minimize->setSizePolicy(sizePolicy4);
        btn_minimize->setMinimumSize(QSize(24, 24));
        btn_minimize->setMaximumSize(QSize(24, 24));
        btn_minimize->setAutoFillBackground(false);
        btn_minimize->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/win_min_px32_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/win_min_px32_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/win_min_px32_pressed.png);\n"
"}"));
        btn_minimize->setAutoDefault(false);
        btn_minimize->setFlat(false);

        horizontalLayout_2->addWidget(btn_minimize);

        btn_maximize = new QPushButton(widget_windows);
        btn_maximize->setObjectName(QString::fromUtf8("btn_maximize"));
        sizePolicy4.setHeightForWidth(btn_maximize->sizePolicy().hasHeightForWidth());
        btn_maximize->setSizePolicy(sizePolicy4);
        btn_maximize->setMinimumSize(QSize(24, 24));
        btn_maximize->setMaximumSize(QSize(24, 24));
        btn_maximize->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/win_max_px32_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/win_max_px32_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/win_max_px32_pressed.png);\n"
"}"));
        btn_maximize->setFlat(false);

        horizontalLayout_2->addWidget(btn_maximize);

        btn_close = new QPushButton(widget_windows);
        btn_close->setObjectName(QString::fromUtf8("btn_close"));
        sizePolicy4.setHeightForWidth(btn_close->sizePolicy().hasHeightForWidth());
        btn_close->setSizePolicy(sizePolicy4);
        btn_close->setMinimumSize(QSize(24, 24));
        btn_close->setMaximumSize(QSize(24, 24));
        btn_close->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/win_close_px32_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/win_close_px32_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/win_close_px32_pressed.png);\n"
"}"));

        horizontalLayout_2->addWidget(btn_close);

        videoWidget = new VideoWidget(MediaPlayerClass);
        videoWidget->setObjectName(QString::fromUtf8("videoWidget"));
        videoWidget->setGeometry(QRect(0, 40, 800, 400));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(videoWidget->sizePolicy().hasHeightForWidth());
        videoWidget->setSizePolicy(sizePolicy5);
        videoWidget->setMinimumSize(QSize(0, 0));
        videoWidget->setMouseTracking(true);
        videoWidget->setFocusPolicy(Qt::StrongFocus);
        videoWidget->setStyleSheet(QString::fromUtf8(""));
        bottom_bar = new QWidget(MediaPlayerClass);
        bottom_bar->setObjectName(QString::fromUtf8("bottom_bar"));
        bottom_bar->setGeometry(QRect(0, 440, 800, 60));
        sizePolicy1.setHeightForWidth(bottom_bar->sizePolicy().hasHeightForWidth());
        bottom_bar->setSizePolicy(sizePolicy1);
        bottom_bar->setMinimumSize(QSize(0, 60));
        bottom_bar->setMaximumSize(QSize(16777215, 60));
        bottom_bar->setMouseTracking(true);
        bottom_bar->setStyleSheet(QString::fromUtf8("background-color: rgb(20, 20, 20);\n"
""));
        hSlider_progress = new Slider(bottom_bar);
        hSlider_progress->setObjectName(QString::fromUtf8("hSlider_progress"));
        hSlider_progress->setGeometry(QRect(0, 0, 800, 6));
        sizePolicy1.setHeightForWidth(hSlider_progress->sizePolicy().hasHeightForWidth());
        hSlider_progress->setSizePolicy(sizePolicy1);
        hSlider_progress->setMinimumSize(QSize(0, 6));
        hSlider_progress->setMaximumSize(QSize(16777215, 6));
        QPalette palette;
        QBrush brush(QColor(20, 20, 20, 255));
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
        hSlider_progress->setPalette(palette);
        QFont font;
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        hSlider_progress->setFont(font);
        hSlider_progress->setFocusPolicy(Qt::StrongFocus);
        hSlider_progress->setContextMenuPolicy(Qt::DefaultContextMenu);
        hSlider_progress->setStyleSheet(QString::fromUtf8("QSlider::groove\n"
"{\n"
"	background-color: rgb(200, 200, 200, 50);\n"
"}\n"
"\n"
"QSlider::handle \n"
"{		\n"
"	width: 10px;\n"
"	background-color: rgb(0, 100, 240);\n"
"}\n"
"\n"
"QSlider::sub-page\n"
"{\n"
"	background-color: rgb(23, 171, 255);\n"
"}"));
        hSlider_progress->setMaximum(999);
        hSlider_progress->setSingleStep(1);
        hSlider_progress->setPageStep(10);
        hSlider_progress->setValue(0);
        hSlider_progress->setOrientation(Qt::Horizontal);
        hSlider_progress->setInvertedAppearance(false);
        hSlider_progress->setInvertedControls(false);
        hSlider_progress->setTickPosition(QSlider::NoTicks);
        hSlider_progress->setTickInterval(0);
        widget_control = new QWidget(bottom_bar);
        widget_control->setObjectName(QString::fromUtf8("widget_control"));
        widget_control->setGeometry(QRect(280, 6, 211, 54));
        widget_control->setMouseTracking(true);
        widget_control->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 0);"));
        horizontalLayout = new QHBoxLayout(widget_control);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btn_stop = new QPushButton(widget_control);
        btn_stop->setObjectName(QString::fromUtf8("btn_stop"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(32);
        sizePolicy6.setVerticalStretch(32);
        sizePolicy6.setHeightForWidth(btn_stop->sizePolicy().hasHeightForWidth());
        btn_stop->setSizePolicy(sizePolicy6);
        btn_stop->setMinimumSize(QSize(32, 32));
        btn_stop->setMaximumSize(QSize(32, 32));
        btn_stop->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/stop_px48_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/stop_px48_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/stop_px48_pressed.png);\n"
"}"));
        btn_stop->setIconSize(QSize(24, 24));
        btn_stop->setFlat(false);

        horizontalLayout->addWidget(btn_stop);

        btn_previous = new QPushButton(widget_control);
        btn_previous->setObjectName(QString::fromUtf8("btn_previous"));
        sizePolicy6.setHeightForWidth(btn_previous->sizePolicy().hasHeightForWidth());
        btn_previous->setSizePolicy(sizePolicy6);
        btn_previous->setMinimumSize(QSize(32, 32));
        btn_previous->setMaximumSize(QSize(32, 32));
        btn_previous->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/previous_px48_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/previous_px48_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/previous_px48_pressed.png);\n"
"}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/MediaPlayer/Resources/previous_24px.ico"), QSize(), QIcon::Normal, QIcon::Off);
        btn_previous->setIcon(icon1);
        btn_previous->setIconSize(QSize(24, 24));
        btn_previous->setFlat(false);

        horizontalLayout->addWidget(btn_previous);

        btn_play = new QPushButton(widget_control);
        btn_play->setObjectName(QString::fromUtf8("btn_play"));
        btn_play->setEnabled(true);
        sizePolicy2.setHeightForWidth(btn_play->sizePolicy().hasHeightForWidth());
        btn_play->setSizePolicy(sizePolicy2);
        btn_play->setMinimumSize(QSize(48, 48));
        btn_play->setMouseTracking(true);
        btn_play->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/play_px48_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/play_px48_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/play_px48_pressed.png);\n"
"}"));
        btn_play->setIconSize(QSize(32, 32));
        btn_play->setFlat(false);

        horizontalLayout->addWidget(btn_play);

        btn_next = new QPushButton(widget_control);
        btn_next->setObjectName(QString::fromUtf8("btn_next"));
        sizePolicy6.setHeightForWidth(btn_next->sizePolicy().hasHeightForWidth());
        btn_next->setSizePolicy(sizePolicy6);
        btn_next->setMinimumSize(QSize(32, 32));
        btn_next->setMaximumSize(QSize(32, 32));
        btn_next->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/next_px48_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/next_px48_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/next_px48_pressed.png);\n"
"}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/MediaPlayer/Resources/next_24px.ico"), QSize(), QIcon::Normal, QIcon::Off);
        btn_next->setIcon(icon2);
        btn_next->setIconSize(QSize(24, 24));
        btn_next->setFlat(false);

        horizontalLayout->addWidget(btn_next);

        btn_sound = new QPushButton(widget_control);
        btn_sound->setObjectName(QString::fromUtf8("btn_sound"));
        sizePolicy6.setHeightForWidth(btn_sound->sizePolicy().hasHeightForWidth());
        btn_sound->setSizePolicy(sizePolicy6);
        btn_sound->setMinimumSize(QSize(32, 32));
        btn_sound->setMaximumSize(QSize(32, 32));
        btn_sound->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/vol_low_px48_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/vol_low_px48_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/vol_low_px48_pressed.png);\n"
"}"));
        btn_sound->setIconSize(QSize(24, 24));
        btn_sound->setFlat(false);

        horizontalLayout->addWidget(btn_sound);

        widget_tool = new QWidget(bottom_bar);
        widget_tool->setObjectName(QString::fromUtf8("widget_tool"));
        widget_tool->setGeometry(QRect(690, 6, 111, 54));
        widget_tool->setMouseTracking(true);
        widget_tool->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 0);"));
        horizontalLayout_3 = new QHBoxLayout(widget_tool);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 9, 0);
        btn_recordList = new QPushButton(widget_tool);
        btn_recordList->setObjectName(QString::fromUtf8("btn_recordList"));
        sizePolicy4.setHeightForWidth(btn_recordList->sizePolicy().hasHeightForWidth());
        btn_recordList->setSizePolicy(sizePolicy4);
        btn_recordList->setMinimumSize(QSize(24, 24));
        btn_recordList->setMaximumSize(QSize(24, 24));
        btn_recordList->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/history_px48_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/history_px48_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/history_px48_pressed.png);\n"
"}"));

        horizontalLayout_3->addWidget(btn_recordList);

        btn_fullScreen = new QPushButton(widget_tool);
        btn_fullScreen->setObjectName(QString::fromUtf8("btn_fullScreen"));
        sizePolicy4.setHeightForWidth(btn_fullScreen->sizePolicy().hasHeightForWidth());
        btn_fullScreen->setSizePolicy(sizePolicy4);
        btn_fullScreen->setMinimumSize(QSize(24, 24));
        btn_fullScreen->setMaximumSize(QSize(24, 24));
        btn_fullScreen->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background: transparent;\n"
"}\n"
"\n"
"QPushButton::!hover\n"
"{\n"
"	image: url(:/normal/Resources/normal/video_full_px48_normal.png);\n"
"}\n"
"\n"
"QPushButton::hover\n"
"{\n"
"	image: url(:/hover/Resources/hover/video_full_px48_hover.png);\n"
"}\n"
"\n"
"QPushButton::pressed\n"
"{\n"
"	image: url(:/pressed/Resources/pressed/video_full_px48_pressed.png);\n"
"}"));

        horizontalLayout_3->addWidget(btn_fullScreen);

        label_playtime = new QLabel(bottom_bar);
        label_playtime->setObjectName(QString::fromUtf8("label_playtime"));
        label_playtime->setGeometry(QRect(10, 6, 161, 54));
        sizePolicy2.setHeightForWidth(label_playtime->sizePolicy().hasHeightForWidth());
        label_playtime->setSizePolicy(sizePolicy2);
        label_playtime->setMouseTracking(true);
        label_playtime->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(0, 0, 0, 0);\n"
"font: 10pt;"));
        comboBox_open = new QComboBox(MediaPlayerClass);
        comboBox_open->addItem(QString());
        comboBox_open->addItem(QString());
        comboBox_open->setObjectName(QString::fromUtf8("comboBox_open"));
        comboBox_open->setGeometry(QRect(340, 270, 100, 40));
        sizePolicy2.setHeightForWidth(comboBox_open->sizePolicy().hasHeightForWidth());
        comboBox_open->setSizePolicy(sizePolicy2);
        comboBox_open->setMouseTracking(true);
        comboBox_open->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"	font: 12pt;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(0, 74, 112);\n"
"	border: 3px solid rgb(0, 74, 112);\n"
"   	border-radius: 10px;\n"
"	padding: 2px 2px 2px 2px\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"    font: 12pt;\n"
"	color: rgb(255, 255, 255);\n"
"}"));
        videoWidget->raise();
        bottom_bar->raise();
        comboBox_open->raise();
        top_bar->raise();

        retranslateUi(MediaPlayerClass);
        QObject::connect(btn_play, SIGNAL(clicked()), MediaPlayerClass, SLOT(playOrPause()));
        QObject::connect(btn_stop, SIGNAL(clicked()), MediaPlayerClass, SLOT(stop()));
        QObject::connect(hSlider_progress, SIGNAL(sliderPressed()), MediaPlayerClass, SLOT(sliderPressed()));
        QObject::connect(hSlider_progress, SIGNAL(sliderReleased()), MediaPlayerClass, SLOT(sliderReleased()));
        QObject::connect(hSlider_progress, SIGNAL(sliderMoved(int)), MediaPlayerClass, SLOT(sliderMoved()));
        QObject::connect(btn_minimize, SIGNAL(clicked()), MediaPlayerClass, SLOT(showMinimized()));
        QObject::connect(btn_close, SIGNAL(clicked()), MediaPlayerClass, SLOT(close()));

        btn_minimize->setDefault(false);


        QMetaObject::connectSlotsByName(MediaPlayerClass);
    } // setupUi

    void retranslateUi(QWidget *MediaPlayerClass)
    {
        MediaPlayerClass->setWindowTitle(QCoreApplication::translate("MediaPlayerClass", "MediaPlayer", nullptr));
        label_logo->setText(QString());
        label_filename->setText(QString());
        btn_minimize->setText(QString());
        btn_maximize->setText(QString());
        btn_close->setText(QString());
        btn_stop->setText(QString());
        btn_previous->setText(QString());
        btn_play->setText(QString());
        btn_next->setText(QString());
        btn_sound->setText(QString());
        btn_recordList->setText(QString());
        btn_fullScreen->setText(QString());
        label_playtime->setText(QCoreApplication::translate("MediaPlayerClass", "00:00:00 / 00:00:00", nullptr));
        comboBox_open->setItemText(0, QCoreApplication::translate("MediaPlayerClass", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
        comboBox_open->setItemText(1, QCoreApplication::translate("MediaPlayerClass", "\346\211\223\345\274\200URL", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MediaPlayerClass: public Ui_MediaPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIAPLAYER_H
