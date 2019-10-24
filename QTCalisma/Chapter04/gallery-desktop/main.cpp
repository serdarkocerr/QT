#include "MainWindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    qDebug()<<"int main(int argc, char *argv[]) --begin line 6  ";

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug()<<"int main(int argc, char *argv[]) --end line 11  ";

    return a.exec();
}
