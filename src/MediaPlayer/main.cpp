#include "MediaPlayer.h"
#include <QtWidgets/QApplication>
#include <QDir>
#include "glog/logging.h"

int main(int argc, char *argv[])
{
    QDir dir;
    dir.mkdir("./log");

    // 使用前添加GOOGLE_GLOG_DLL_DECL=
    google::InitGoogleLogging((const char*)argv[0]); // 初始化glog
    google::SetLogDestination(google::GLOG_INFO, "./log/info");
    FLAGS_logbufsecs = 0;

    QApplication a(argc, argv);
    MediaPlayer w;
    w.show();

    return a.exec();
}
