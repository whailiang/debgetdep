#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QObject>

class MyThread :public QThread
{
    Q_OBJECT

public:
    MyThread();
    ~MyThread();

    QString download_deb_name;

    void set_debname(QString debname);


protected:
    void run();


};

#endif // MYTHREAD_H
