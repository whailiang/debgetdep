#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include "mythread.h"

extern char *deb_name;
#define MAX_LINE 1024

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void get_dep(char *filename);
    void download_deb();
    void ar_deb();
    void rm_deb();
    void postinst();
    void preinst();

    void clear_file();
    void tar_file();
    void del_file(QString filename);

    char buf[MAX_LINE];

    QList<QString> list_dep;
    QStringList list;

    MyThread *download_thread[20];

    int num = 0;



private:

};
#endif // WIDGET_H
