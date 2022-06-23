#define _USE_MATH_DEFINES
#include <cmath>
#include "mainwindow.h"
#include "qgsapplication.h"
#include <QApplication>
#include <qgsmapcanvas.h>
#include <QVBoxLayout>

#include <QApplication>
#include <qgsapplication.h>
#include <mainwindow.h>
#include <QDebug>


#include <qgsapplication.h>
#include <qgsproviderregistry.h>

#include <qgssinglesymbolrenderer.h>
#include <qgsmaplayer.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgsproject.h>
#include <qgssymbol.h>

void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }
    QFile outFile("D:/Qt5.12.12/workspace/OUTPUTS/log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    QgsApplication app(argc, argv, true);
    qInstallMessageHandler(myMessageHandler);

    QgsApplication::setPrefixPath("D:/OSGeo4W/apps/qgis-ltr-dev",true);
    QgsApplication::initQgis();

   //QApplication a(argc, argv);



    MainWindow w;
    w.show();
//    w.loadMap();
    w.raise();
    w.loadMap();

    int result = app.exec();

    QgsApplication::exitQgis();
    return result;
//    return a.exec();

//    QgsApplication app(argc, argv, true);
//    QgsApplication::setPrefixPath("D:/OSGeo4W/apps/qgis-ltr-dev",true);

//    app.init();

//    QVBoxLayout *vlayout = new QVBoxLayout;
//    vlayout->setContentsMargins(0,0,0,0);

//    QgsMapCanvas * mypMapCanvas = new QgsMapCanvas();
//    mypMapCanvas->setCanvasColor(Qt::white);
//    mypMapCanvas->show();

//    vlayout->addWidget(mypMapCanvas);


//   int rsult =  app.exec();

//   // app.exitQgis();

//    return rsult;

}
