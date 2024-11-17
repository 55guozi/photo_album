#include "mainwindow.h"

#include <QApplication>
#include <qfile.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/style.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("qss file open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("qss file open failed");
    }
    MainWindow w;
    w.setWindowTitle("Album");
    w.showMaximized();
    return a.exec();
}
