#include "mythread.h"
#include <QProcess>
#include <QDebug>

MyThread::MyThread()
    : QThread()
{


}
MyThread::~MyThread()
{

}

void MyThread::run()
{
    QProcess process;
    QString download_cmd;
    download_cmd.append("apt download ");
    download_cmd.append(download_deb_name);
    qDebug()<<"----"<<download_cmd;
    process.start(download_cmd);
    process.waitForFinished(-1);
    process.close();
    download_cmd.clear();
}

void MyThread::set_debname(QString debname)
{
    download_deb_name.clear();
    download_deb_name.append(debname);
}
