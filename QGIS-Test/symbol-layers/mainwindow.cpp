#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qgis_core.h>
#include <qgis_gui.h>
#include <qgis.h>
#include <qgssymbol.h>
#include <qgslinesymbol.h>
#include <qgsrasterlayer.h>
#include <qgssinglesymbolrenderer.h>
#include <qgssimplifymethod.h>
#include <qgssimplelinematerialsettings.h>
#include <qgslinesymbollayer.h>
#include <qgsgeometry.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mapCanvas = new QgsMapCanvas();
    mapCanvas->setCanvasColor(Qt::white);

    playout  = new QVBoxLayout;
    playout->setContentsMargins(0,0,0,0);
    playout->addWidget(mapCanvas);

    centralWidget()->setLayout(playout);

    compassRose = new CompassRoseItem(mapCanvas);
    compassRose->setCenter(QgsPoint(150,400));
    compassRose->setSize(80);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMap()
{

//    QgsSymbol *symbol = QgsLineSymbol::createSimple({});
//    symbol->deleteSymbolLayer(0);

//    QgsSimpleLineSymbolLayer *symbol_layer = new QgsSimpleLineSymbolLayer();
//    symbol_layer->setWidth(4);
//    symbol_layer->setColor(QColor("light gray"));
//    symbol_layer->setPenCapStyle(Qt::FlatCap);
//    symbol->appendSymbolLayer(symbol_layer);


//    QgsSimpleLineSymbolLayer *symbol_layer2 = new QgsSimpleLineSymbolLayer();
//    symbol_layer2->setWidth(2);
//    symbol_layer2->setColor(QColor("black"));
//    symbol_layer2->setPenCapStyle(Qt::FlatCap);
//    symbol->appendSymbolLayer(symbol_layer2);


//    QgsSimpleLineSymbolLayer *symbol_layer3 = new QgsSimpleLineSymbolLayer();
//    symbol_layer3->setWidth(1);
//    symbol_layer3->setColor(QColor("white"));
//    symbol_layer3->setPenStyle(Qt::DotLine);
//    symbol->appendSymbolLayer(symbol_layer3);

//    QgsSingleSymbolRenderer *renderer = new QgsSingleSymbolRenderer(symbol);
//    //mapCanvas->layer(0)->setRen
//    // mapCanvas->currentLayer()->symbol
//    QString currentDirShp ="D:/Qt5.12.12/workspace/Lex-Example/data/ne_10m_populated_places.shp";
//    QgsVectorLayer *lVector;
//    lVector = new QgsVectorLayer(currentDirShp,"landmarks","ogr");
//    QgsProject::instance()->addMapLayer(lVector);
//    lVector->setRenderer(renderer);
//    lVector->setLabelsEnabled(true);

//    QList <QgsMapLayer *> layers;
//    layers.append(lVector);
//    mapCanvas->setLayers(layers);
//    mapCanvas->setVisible(true);
//    mapCanvas->setExtent(QgsRectangle(-127.7,24.4,-79.3,49.1));
//    mapCanvas->refresh();


    /** Compass ***/

    QString currentDirtfw="D:/Qt5.12.12/workspace/Lex-Example/data/NE1_HR_LC_SR_W_DR.tif";
    lRaster = new QgsRasterLayer(currentDirtfw,"basemap");
    QgsProject::instance()->addMapLayer(lRaster);

    /** Memory based layer**/
    qDebug()<<"map loading....  Memory based layer - begin";

    lMemoryBased = new QgsVectorLayer("Point?crs=EPSG:4326&field=height:double&field=name:string(255)&index=yes", "Point Layer", "memory");
    QgsVectorDataProvider *provider = lMemoryBased->dataProvider();
    QgsProject::instance()->addMapLayer(lMemoryBased);

    QgsFields fields = provider->fields();
    QList <QgsFeature > features;

    QgsFeature feature;
    feature.setGeometry(QgsGeometry::fromWkt("POINT (2.2945 48.8582)"));

    feature.setFields(fields);
    feature.setAttribute("height", 301);
    feature.setAttribute("placeName", QString("Eiffel Tower"));
    features.append(feature);

    QgsFeature feature2;
    feature2.setGeometry(QgsGeometry::fromWkt("POINT (0.0761 51.5081)"));
    feature2.setFields(fields);
    feature2.setAttribute("height", 27);
    feature2.setAttribute("placeName", QString("Tower of London"));
    features.append(feature2);

    QgsFeature feature3;
    feature3.setGeometry(QgsGeometry::fromWkt("POINT (10.3964 43.7231)"));
    feature3.setFields(fields);
    feature3.setAttribute("height", 56);
    feature3.setAttribute("placeName", QString("Leaning Tower of Pisa"));
    features.append(feature3);

    provider->addFeatures(features);
    lMemoryBased->updateExtents();


    QList <QgsMapLayer *> layers;
    layers.append(lMemoryBased);
    layers.append(lRaster);
    mapCanvas->setLayers(layers);
    mapCanvas->setVisible(true);
    mapCanvas->setExtent(QgsRectangle(-127.7,24.4,-79.3,49.1));
    mapCanvas->refresh();

    qDebug()<<"map loading....  Memory based layer - end";

}

