#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    deb_name = argv[1];
    Widget w;
    return a.exec();
}
