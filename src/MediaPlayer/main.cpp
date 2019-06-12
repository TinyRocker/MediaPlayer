#include "MediaPlayer.h"
#include "glog/logging.h"
#include <QtWidgets/QApplication>
#include <QThread>

#include "MediaPlayThread.h"
#include "VideoPlayInterface.h"
#include "VideoWidget.h"

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swresample.lib")

#ifdef _DEBUG
#pragma comment(lib, "glogd.lib") 
#else
#pragma comment(lib, "glog.lib")
#endif // _DEBUG

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    // 使用前添加GOOGLE_GLOG_DLL_DECL=
    google::InitGoogleLogging((const char*)argv[0]); // 初始化glog
    google::SetLogDestination(google::GLOG_INFO, "./log/info");
    FLAGS_logbufsecs = 0;

    QApplication a(argc, argv);
    MediaPlayer w;
    w.show();

    return a.exec();
}
