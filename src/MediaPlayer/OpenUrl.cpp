#include "OpenUrl.h"
#include "MediaPlayer.h"

#include <iostream>
using namespace std;

OpenUrl::OpenUrl(QWidget *parent) : QDialog(parent)
{
    connect(this, SIGNAL(isAccept()), parent, SLOT(openUrl()));
    connect(this, SIGNAL(clearUrlData()), parent, SLOT(clearUrlData()));
}

OpenUrl::~OpenUrl()
{
}

void OpenUrl::accept()
{
    QDialog::accept();
    isAccept();
}

void OpenUrl::reject()
{
    QDialog::reject();
}

void OpenUrl::clearUrl()
{
    clearUrlData();
}
