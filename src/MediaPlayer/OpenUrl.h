#pragma once
#include <QDialog>
#include "ui_OpenUrl.h"

class OpenUrl : public QDialog
{
    Q_OBJECT

public:
    OpenUrl(QWidget *parent);
    virtual ~OpenUrl();
    //bool isAccept() { return m_isAccept; };
signals:
    void isAccept();
    void clearUrlData();

public slots:
    void accept();
    void reject();
    void clearUrl();
private:
    //bool m_isAccept = false;
};

