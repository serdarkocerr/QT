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

int main(int argc, char *argv[])
{
    QgsApplication app(argc, argv, true);
    QgsApplication::setPrefixPath("D:/OSGeo4W/apps/qgis-ltr-dev",true);
    QgsApplication::initQgis();

    QApplication a(argc, argv);



    MainWindow w;
    w.show();
//    w.loadMap();
    w.raise();


    int result = app.exec();


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
