#define _USE_MATH_DEFINES
#include <cmath>
#include "mainwindow.h"
#include "qgsapplication.h"
#include <QApplication>
#include <qgsmapcanvas.h>
#include <QVBoxLayout>

#include <QApplication>
#include <qgssymbol.h>

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

    MainWindow w;
    w.show();
    w.loadMap();
    int result = app.exec();

    QgsApplication::exitQgis();

    return result;

}
