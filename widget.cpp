#include "widget.h"
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <QProcess>
#include <QDir>
#include <unistd.h>

extern char *deb_name = "";

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    get_dep(deb_name);
    download_deb();
    ar_deb();
    rm_deb();
    clear_file();

    tar_file();

    exit(0);
}

Widget::~Widget()
{

}
void Widget::tar_file()
{
    QProcess cp_shell;
    QString cp_cmd;

    cp_cmd.append("cp -r bin/ usr/");
    cp_shell.start(cp_cmd);
    cp_shell.waitForFinished();
    cp_cmd.clear();
    cp_cmd.append("cp -r sbin/ usr/");
    cp_shell.start(cp_cmd);
    cp_shell.waitForFinished();
    cp_cmd.clear();
    cp_cmd.append("cp -r lib/ usr/");
    cp_shell.start(cp_cmd);
    cp_shell.waitForFinished();
    cp_cmd.clear();
    cp_cmd.append("cp -r lib64/ usr/");
    cp_shell.start(cp_cmd);
    cp_shell.waitForFinished();
    cp_cmd.clear();

    QString filename ="ld.so.conf";
    QDir dir;
    if(!dir.exists(deb_name)){
        dir.mkdir(deb_name);
    }
    dir=deb_name;
    QString path = dir.filePath(filename);
    QFile file;
    file.setFileName(path);

    if(file.open(QIODevice::WriteOnly |QIODevice::Text |QIODevice::Append)){
        file.write("/app/usr/lib\n");
        file.write("/app/usr/lib/x86_64-linux-gnu\n");
    }
    file.close();

    cp_cmd.append("cp -r usr/ ");
    cp_cmd.append(deb_name);
    cp_shell.start(cp_cmd);
    cp_shell.waitForFinished();
    qDebug()<<"========"<<cp_cmd;
    cp_cmd.clear();

    cp_cmd.append("cp -r etc/ ");
    cp_cmd.append(deb_name);
    cp_shell.start(cp_cmd);
    cp_shell.waitForFinished();
    qDebug()<<"========"<<cp_cmd;
    cp_cmd.clear();

    cp_cmd.append("cp -r var/ ");
    cp_cmd.append(deb_name);
    cp_shell.start(cp_cmd);
    cp_shell.waitForFinished();
    qDebug()<<"========"<<cp_cmd;
    cp_cmd.clear();

    cp_cmd.append("tar czf ");
    cp_cmd.append(deb_name);
    cp_cmd.append(".tar  ");
    cp_cmd.append(deb_name);

    cp_shell.start(cp_cmd);
    cp_shell.waitForFinished();
    qDebug()<<"========"<<cp_cmd;
    cp_cmd.clear();

}
void Widget::del_file(QString filename)
{
    QProcess del_file_shell;
    QString del_file_cmd;

    del_file_cmd.append("rm ");
    del_file_cmd.append(filename);

    qDebug()<<del_file_cmd;

    del_file_shell.start(del_file_cmd);
    del_file_shell.waitForFinished();
    del_file_cmd.clear();

}

void Widget::clear_file()
{
    //设置要遍历的目录
    QDir dir(QDir::currentPath());
    //将过滤后的文件名称存入到files列表中
    QList<QString> files = dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);

    for(int i = 0;i<files.length();i++)
    {
        QFileInfo fi(QDir::currentPath() + "/" +files.at(i));
        if(!fi.isDir()){
            if (files.at(i).compare("getdep") !=0 ){
                del_file(files.at(i));
            }
        }
    }

}
void Widget::rm_deb()
{
    QProcess rm_deb_shell;
    QString rm_deb_cmd;
    for(int i = 0; i < list.length();i++)
    {
        rm_deb_cmd.append("rm ");
        rm_deb_cmd.append(list.at(i));
        qDebug()<<rm_deb_cmd;
        rm_deb_shell.start(rm_deb_cmd);
        rm_deb_shell.waitForFinished();
        rm_deb_cmd.clear();
    }
}
//判断当前目录有没有 preinst脚本 如果有，修改脚本中内容，执行
void Widget::preinst()
{
    QFileInfo preinst_file("preinst");
    QProcess preinst_shell;
    QString preinst_cmd;

    if(preinst_file.exists()){
        //修改脚本
        preinst_cmd.append("sed -i 's/usr/");
        preinst_cmd.append(QDir::currentPath().right(QDir::currentPath().length()-1));
        preinst_cmd.append("/usr/g' preinst");
        preinst_shell.start(preinst_cmd);
        preinst_shell.waitForFinished();
        preinst_cmd.clear();

        preinst_cmd.append("sed -i 's/etc/");
        preinst_cmd.append(QDir::currentPath().right(QDir::currentPath().length()-1));
        preinst_cmd.append("/etc/g' preinst");
        preinst_shell.start(preinst_cmd);
        preinst_shell.waitForFinished();
        preinst_cmd.clear();

        qDebug()<<preinst_cmd;
        //执行脚本
        preinst_cmd.append("./preinst");
        preinst_shell.start(preinst_cmd);
        preinst_shell.waitForFinished();
        preinst_cmd.clear();

        //执行后删除脚本
        preinst_cmd.append("rm preinst");
        preinst_shell.start(preinst_cmd);
        preinst_shell.waitForFinished();
        preinst_cmd.clear();
    }

}


//判断当前目录有没有postinst 脚本 如果有，修改脚本中内容，执行
void Widget::postinst()
{
    QFileInfo postinst_file("postinst");

    QProcess postinst_shell;
    QString postinst_cmd;


    if(postinst_file.exists()){
        //修改脚本
        postinst_cmd.append("sed -i 's/usr/");
        postinst_cmd.append(QDir::currentPath().right(QDir::currentPath().length()-1));
        postinst_cmd.append("/usr/g' postinst");
        postinst_shell.start(postinst_cmd);
        postinst_shell.waitForFinished();
        qDebug()<<postinst_cmd;
        postinst_cmd.clear();

        postinst_cmd.append("sed -i 's/etc/");
        postinst_cmd.append(QDir::currentPath().right(QDir::currentPath().length()-1));
        postinst_cmd.append("/etc/g' postinst");
        postinst_shell.start(postinst_cmd);
        postinst_shell.waitForFinished();
        qDebug()<<postinst_cmd;
        postinst_cmd.clear();

        //执行脚本
        postinst_cmd.append("./postinst");
        postinst_shell.start(postinst_cmd);
        postinst_shell.waitForFinished();
        postinst_cmd.clear();

        //执行后删除脚本
        postinst_cmd.append("rm postinst");
        postinst_shell.start(postinst_cmd);
        postinst_shell.waitForFinished();
        postinst_cmd.clear();

    }
}
void Widget::ar_deb()
{
    int deb_num = 0;
    //获取文件夹下的文件名称
    QString path = "./";
    QDir dir(path);
    if (!dir.exists()) {
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList filters;
    filters << "*.deb";
    dir.setNameFilters(filters);
    list = dir.entryList();
    if (list.count() <= 0)
    {
        return;
    }
    QProcess process_dep;
    QString ar_deb_cmd;
    QString tar_cmd1;
    QString tar_cmd2;
    QString tar_cmd3;

    QString control_cmd1;
    QString control_cmd2;
    QString control_cmd3;


    for(int j = 0;j<list.length();j++)
    {
        ar_deb_cmd.append("ar -vx ");
        ar_deb_cmd.append(list.at(j));
        qDebug()<<ar_deb_cmd;
        process_dep.start(ar_deb_cmd);
        process_dep.waitForFinished();
        ar_deb_cmd.clear();
        QByteArray result = process_dep.readAllStandardOutput();

        //执行postinst  preinst
        if(result.contains("control.tar.xz")){
            control_cmd1.append("xz -d data.tar.xz");
            control_cmd2.append("tar xvf control.tar");
            control_cmd3.append("rm control.tar");

            preinst();

            process_dep.start(control_cmd1);
            process_dep.waitForFinished();
            process_dep.start(control_cmd2);
            process_dep.waitForFinished();

            process_dep.start(control_cmd3);
            process_dep.waitForFinished();
            qDebug()<<control_cmd1;
            qDebug()<<control_cmd2;
            qDebug()<<control_cmd3;
            control_cmd1.clear();
            control_cmd2.clear();
            control_cmd3.clear();
        }else if(result.contains("control.tar.gz")){
            control_cmd1.append("tar xvf control.tar.gz");

            preinst();

            process_dep.start(control_cmd1);
            process_dep.waitForFinished();

            qDebug()<<control_cmd1;
            control_cmd1.clear();
        }
        //xz
        if(result.contains("data.tar.xz")){
            qDebug()<<"xz";
            tar_cmd1.append("xz -d data.tar.xz ");
            tar_cmd2.append("tar xvf data.tar");
            tar_cmd3.append("rm data.tar debian-binary");
            process_dep.start(tar_cmd1);
            process_dep.waitForFinished();
            process_dep.start(tar_cmd2);
            process_dep.waitForFinished();
            process_dep.start(tar_cmd3);
            process_dep.waitForFinished();

            postinst();

            qDebug()<<tar_cmd1;
            qDebug()<<tar_cmd2;
            qDebug()<<tar_cmd3;
            tar_cmd1.clear();
            tar_cmd2.clear();
            tar_cmd3.clear();
        }else if(result.contains("data.tar.gz")){
            qDebug()<<"gz";
            tar_cmd1.append("tar xvf data.tar.gz");
            tar_cmd2.append("rm data.tar debian-binary");
            process_dep.start(tar_cmd1);
            process_dep.waitForFinished();
            process_dep.start(tar_cmd2);
            process_dep.waitForFinished();

            postinst();

            qDebug()<<tar_cmd1;
            qDebug()<<tar_cmd2;
            tar_cmd1.clear();
            tar_cmd2.clear();
        }
        qDebug()<<"解压"<<list.at(j);
        deb_num++;
    }
    qDebug()<<"deb个数："<<deb_num;

}
void Widget::get_dep(char *name)
{
    char getDeplistcmd[128];
    sprintf(getDeplistcmd,"apt-cache depends %s | grep '依赖'| cut -d: -f2 | tr -d '<>'",name);

    FILE * p_file = NULL;

    p_file = popen(getDeplistcmd, "r");
    if (!p_file) {
        fprintf(stderr, "Erro to popen");
    }
    int num_dep = 1;
    while (fgets(buf, MAX_LINE, p_file) != NULL) {
        //去掉换行符
        if(buf == NULL){
            break;
        }
        buf[strlen(buf)-1] = '\0';
        if(!list_dep.contains(buf)){
            list_dep.append(buf);
        }
        num_dep++;
    }
    qDebug()<<name<<"依赖个数"<<num_dep;
    qDebug()<<"全部依赖个数"<<list_dep.length();
    pclose(p_file);
    if(num <= list_dep.length()-1){
        QByteArray ba = list_dep.at(num).toLatin1();
        char * name_dep = ba.data();
        num++;
        get_dep(name_dep);
    }
    else
        qDebug()<<"end";
}

void Widget::download_deb()
{
    list_dep.insert(0,deb_name);

    QProcess process;
    QString download_cmd;
    for(int i = 0; i < list_dep.length();i++){
        download_cmd.append("apt download ");
        download_cmd.append(list_dep.at(i));
        qDebug()<<"----"<<download_cmd;
        process.start(download_cmd);
        process.waitForFinished();
        if(!process.waitForFinished(3000))
        {
            qDebug()<<"=============";
        }
        download_cmd.clear();
        qDebug()<<"----------"<<i;
    }
    qDebug()<<list_dep.length();
    qDebug()<<"download end";
}

