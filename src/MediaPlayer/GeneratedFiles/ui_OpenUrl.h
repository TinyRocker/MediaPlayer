/********************************************************************************
** Form generated from reading UI file 'OpenUrl.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENURL_H
#define UI_OPENURL_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogOpenUrl
{
public:
    QLabel *label_1;
    QLabel *label_2;
    QComboBox *comboBox_url;
    QLabel *label_3;
    QPushButton *pushButton_clear;
    QDialogButtonBox *buttonBox_url;

    void setupUi(QDialog *DialogOpenUrl)
    {
        if (DialogOpenUrl->objectName().isEmpty())
            DialogOpenUrl->setObjectName(QString::fromUtf8("DialogOpenUrl"));
        DialogOpenUrl->resize(377, 162);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/MediaPlayer/Resources/MediaPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
        DialogOpenUrl->setWindowIcon(icon);
        label_1 = new QLabel(DialogOpenUrl);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setGeometry(QRect(20, 10, 340, 20));
        label_1->setStyleSheet(QString::fromUtf8(""));
        label_2 = new QLabel(DialogOpenUrl);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 30, 340, 20));
        label_2->setStyleSheet(QString::fromUtf8(""));
        comboBox_url = new QComboBox(DialogOpenUrl);
        comboBox_url->addItem(QString());
        comboBox_url->setObjectName(QString::fromUtf8("comboBox_url"));
        comboBox_url->setGeometry(QRect(50, 60, 310, 20));
        comboBox_url->setEditable(true);
        label_3 = new QLabel(DialogOpenUrl);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 60, 30, 20));
        label_3->setStyleSheet(QString::fromUtf8(""));
        pushButton_clear = new QPushButton(DialogOpenUrl);
        pushButton_clear->setObjectName(QString::fromUtf8("pushButton_clear"));
        pushButton_clear->setGeometry(QRect(30, 123, 60, 24));
        buttonBox_url = new QDialogButtonBox(DialogOpenUrl);
        buttonBox_url->setObjectName(QString::fromUtf8("buttonBox_url"));
        buttonBox_url->setGeometry(QRect(160, 120, 160, 30));
        buttonBox_url->setOrientation(Qt::Horizontal);
        buttonBox_url->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(DialogOpenUrl);
        QObject::connect(buttonBox_url, SIGNAL(accepted()), DialogOpenUrl, SLOT(accept()));
        QObject::connect(buttonBox_url, SIGNAL(rejected()), DialogOpenUrl, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogOpenUrl);
    } // setupUi

    void retranslateUi(QDialog *DialogOpenUrl)
    {
        DialogOpenUrl->setWindowTitle(QApplication::translate("DialogOpenUrl", "\346\211\223\345\274\200URL", nullptr));
        label_1->setText(QApplication::translate("DialogOpenUrl", "\350\276\223\345\205\245\347\275\221\347\273\234\344\270\212\347\232\204url\345\234\260\345\235\200\346\210\226\350\267\257\345\276\204\357\274\214\346\224\257\346\214\201http https", nullptr));
        label_2->setText(QApplication::translate("DialogOpenUrl", "\344\276\213\345\246\202\357\274\232rtmp://live.hkstv.hk.lxdns.com/live/hks2", nullptr));
        comboBox_url->setItemText(0, QApplication::translate("DialogOpenUrl", "rtmp://live.hkstv.hk.lxdns.com/live/hks2", nullptr));

        label_3->setText(QApplication::translate("DialogOpenUrl", "URL:", nullptr));
        pushButton_clear->setText(QApplication::translate("DialogOpenUrl", "\346\270\205\351\231\244\350\256\260\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogOpenUrl: public Ui_DialogOpenUrl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENURL_H
