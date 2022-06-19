#define _USE_MATH_DEFINES
#include <cmath>
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
//    QApplication a(argc, argv);
//        qDebug()<<"TEST1";
//    //QgsApplication app(argc, argv, true);


//    qDebug()<<"TEST2";
//    //return app.exec();
//    return a.exec();

//    // 1. Initialize App
//    QApplication app(argc, argv);
////    QgsApplication::setPrefixPath("D:/OSGeo4W/bin", true);
//    QString PREFIX_PATH = "D:\\OSGeo4W\\apps\\qgis";
//    QgsApplication::setPrefixPath(PREFIX_PATH);
////    D:/OSGeo4W/apps/qgis
//    QgsApplication::initQgis();


//    //2. App start
////    MainWindow win(config, db);
////    win.show();
//    int result = app.exec();

//    //3. Qgis free libs
//    QgsApplication::exitQgis();

//    //4. And say goodby
//    return result;



//    QgsApplication app(argc,argv,true);
//    QgsApplication::setPrefixPath("D:/OSGeo4W/apps/qgis-ltr-dev",true);
//    QgsApplication::initQgis();
//    MainWindow win;
//    win.show();

//    int result = app.exec();
//    QgsApplication::exitQgis();

//    return result;

    // Start the Application
      QgsApplication app(argc, argv, true);
      QgsApplication::setPrefixPath("D:/OSGeo4W/apps/qgis-ltr-dev",true);


      QString myPluginsDir        = "D:/OSGeo4W/apps/qgis-ltr-dev/plugins";
      QString myLayerPath         = "D:/Qt5.12.12/workspace/QGIS-applications/Bairros_Distritos_CidadeSP/LAYER_DISTRITO";
      QString myLayerBaseName     = "DEINFO_DISTRITO";
      QString myProviderName      = "ogr";
      // Instantiate Provider Registry
      QgsProviderRegistry::instance(myPluginsDir);
      // create a maplayer instance
      QgsVectorLayer * mypLayer =new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);
      if (!mypLayer->isValid())
          qDebug()<<"Layer is not valid.";



      QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(QgsSymbol::defaultSymbol(mypLayer->geometryType()));

//      QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(mypLayer->geometryType())
      QList <QgsMapCanvas> myLayerSet;
      mypLayer->setRenderer(mypRenderer);
      if (mypLayer->isValid())
      {
        qDebug("Layer is valid");
      }
      else
      {
        qDebug("Layer is NOT valid");
      }

      // Add the Vector Layer to the Layer Registry
      QgsProject::instance()->addMapLayer(mypLayer);
      QList <QgsMapLayer *> layers;
      // Add the Layer to the Layer Set
      layers.append(mypLayer);

      // Create the Map Canvas
      QgsMapCanvas * mypMapCanvas = new QgsMapCanvas();
      mypMapCanvas->setExtent(mypLayer->extent());
      mypMapCanvas->enableAntiAliasing(true);
      mypMapCanvas->setCanvasColor(QColor(255, 255, 255));
      mypMapCanvas->freeze(false);
      // Set the Map Canvas Layer Set
      mypMapCanvas->setLayers(layers);
      mypMapCanvas->setVisible(true);
      mypMapCanvas->refresh();

      // Start the Application Event Loop
      return app.exec();


}
