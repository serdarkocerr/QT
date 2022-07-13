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


/**

The vector layer class, QgsVectorLayer, includes support for keeping track of the user's current selection.
memory based layer is vector layer so that we can utilize QgsVectorLayer supports.

**/

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

    selectTool = new SelectTool(mapCanvas);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMap()
{


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

void MainWindow::setSelectTool()
{

    this->mapCanvas->setMapTool(selectTool);


}

void SelectTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{

    try {
        qDebug()<<"canvasReleaseEvent....  begin";

        QList<QgsMapToolIdentify::IdentifyResult> found_features =
                this->identify(e->x(), e->y() , TopDownStopAtFirst, VectorLayer);
        if(found_features.size() > 0){
            QgsVectorLayer *layer = (QgsVectorLayer *)found_features.at(0).mLayer;
            QgsFeature feature = found_features.at(0).mFeature;

            if(e->modifiers() & Qt::ShiftModifier){
                layer->select(feature.id());
                qDebug()<<"canvasReleaseEvent....  shift pressed!!!";

            }else {
                //layer.setSelectedFeatures([feature.id()]) --> deprecated

                QgsFeatureIds ids;
                ids.insert(feature.id());
                layer->selectByIds(ids);
                qDebug()<<"canvasReleaseEvent....  shift NOT pressed!!!";

            }
        }else{
            qDebug()<<"canvasReleaseEvent.... clicked BLANK AREA!!!";

             QList<QgsMapLayer *> layers = this->window->layers();
    //        for(QList<QgsMapLayer *>::iterator iter =  layers.begin(); iter != layers.end(); ++iter ){
    //                (*iter)

    //         }
            // Find vector layer and deselect all.
            for(int i = 0; i< layers.size(); i++){
                QgsMapLayer * ret = layers.at(i);
                if(QgsVectorLayer* t = dynamic_cast<QgsVectorLayer*>(ret)){
                        t->selectByIds(QgsFeatureIds());
                        qDebug()<<"canvasReleaseEvent.... clicked BLANK AREA --- deselect all selected!!!! ";

                }

            }
        }
        qDebug()<<"canvasReleaseEvent....  end";
    }  catch (std::exception &e) {
        qDebug() << "Exception : " << e.what();
    }


}

SelectTool::SelectTool(QgsMapCanvas *canvas) : QgsMapToolIdentify(canvas)
{

    this->window = canvas;
    this->setCursor(Qt::ArrowCursor);
}
