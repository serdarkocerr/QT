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
#include <QToolBar>
#include <qmessagebox.h>
#include <qgsrubberband.h>
#include <qgraphicsitem.h>
#include <QtCore/QtMath>
#include <qgsfeaturerequest.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentEditingLayer = 0;

    ui->centralwidget->setMinimumSize(800,400);
    this->setWindowTitle("Geometry Editor");

    createAndConnectToolbar();

    mapCanvas = new QgsMapCanvas();
    mapCanvas->setCanvasColor(Qt::white);

    playout  = new QVBoxLayout;
    playout->setContentsMargins(0,0,0,0);
    playout->addWidget(mapCanvas);

    centralWidget()->setLayout(playout);





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createAndConnectToolbar()
{

    QMenu *fileMenu = this->menuBar()->addMenu("File");
    QMenu *viewMenu = this->menuBar()->addMenu("View");
    QMenu *modeMenu = this->menuBar()->addMenu("Mode");

    QToolBar *toolbar  = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea, toolbar);


    QIcon iconZoomIn(":/icon/mActionZoomIn.png");
    QAction *actionZoomIn = new QAction(iconZoomIn,"ZoomIn",this);
    actionZoomIn->setShortcut(QKeySequence::ZoomIn);


    QIcon iconZoomOut(":/icon/mActionZoomOut.png");
    QAction *actionZoomOut = new QAction(iconZoomOut,"ZoomOut",this);
    actionZoomOut->setShortcut(QKeySequence::ZoomOut);

    QIcon iconPan(":/icon/mActionPan.png");
    actionPan = new QAction(iconPan,"Pan",this);
    actionPan->setShortcut(QKeySequence("Ctrl+1"));
    actionPan->setCheckable(true);


    QIcon iconEditPoint(":/icon/mActionEditPoint.svg");
    actionEditPoint = new QAction(iconEditPoint,"Edit Points",this);
    actionEditPoint->setCheckable(true);

    QIcon iconEditLines(":/icon/mActionEditLine.svg");
    actionEditLine = new QAction(iconEditLines,"Edit Lines",this);
    actionEditLine->setCheckable(true);

    QIcon iconEditPolygons(":/icon/mActionEditPolygon.svg");
    actionEditPolygon = new QAction(iconEditPolygons,"Edit Polygons",this);
    actionEditPolygon->setCheckable(true);

    QIcon iconAddFeature(":/icon/mActionAdd.svg");
    actionAdd = new QAction(iconAddFeature,"Add Feature",this);
    actionAdd->setCheckable(true);

    QIcon iconEditFeature(":/icon/mActionEdit.png");
    actionEdit = new QAction(iconEditFeature,"Edit Feature",this);
    actionEdit->setCheckable(true);

    QIcon iconDeleteFeature(":/icon/mActionDelete.svg");
    actionDelete = new QAction(iconDeleteFeature,"Delete Feature",this);
    actionDelete->setCheckable(true);


    actionQuit = new QAction("Quit",this);
    actionQuit->setShortcut(QKeySequence::Quit);

    /** Menu */
    fileMenu->addAction(actionQuit);

    viewMenu->addAction(actionZoomIn);
    viewMenu->addAction(actionZoomOut);

    modeMenu->addAction(actionPan);
    modeMenu->addAction(actionEditPoint);
    modeMenu->addAction(actionEditLine);
    modeMenu->addAction(actionEditPolygon);
    modeMenu->addAction(actionAdd);
    modeMenu->addAction(actionEdit);
    modeMenu->addAction(actionDelete);



    /** Toolbar */
    toolbar->addAction(actionZoomIn);
    toolbar->addAction(actionZoomOut);
    toolbar->addAction(actionPan);

    toolbar->addAction(actionEditPoint);
    toolbar->addAction(actionEditLine);
    toolbar->addAction(actionEditPolygon);

    toolbar->addAction(actionAdd);
    toolbar->addAction(actionEdit);
    toolbar->addAction(actionDelete);

    this->resize(this->sizeHint());


    connect(actionQuit,SIGNAL(triggered()),this,SLOT(quitSLOT()));
    connect(actionZoomIn,SIGNAL(triggered()),this,SLOT(zoomInSLOT()));
    connect(actionZoomOut,SIGNAL(triggered()),this,SLOT(zoomOutSLOT()));
    connect(actionPan,SIGNAL(triggered()),this,SLOT(setPanModeSLOT()));

    //type
    connect(actionEditPoint,SIGNAL(triggered()),this,SLOT(editPointLayerSLOT()));
    connect(actionEditLine,SIGNAL(triggered()),this,SLOT(editLineLayerSLOT()));
    connect(actionEditPolygon,SIGNAL(triggered()),this,SLOT(editPolygonLayerSLOT()));

    //user action
    connect(actionAdd,SIGNAL(triggered()),this,SLOT(addFeatureSLOT()));
    connect(actionEdit,SIGNAL(triggered()),this,SLOT(editFeatureSLOT()));
    connect(actionDelete,SIGNAL(triggered()),this,SLOT(deleteFeatureSLOT()));




}

void MainWindow::loadMap()
{
    try {
        qDebug()<<"map loading.... begin";
        QString currentDirtfw="D:/Qt5.12.12/workspace/Lex-Example/data/NE1_HR_LC_SR_W_DR.tif";
        lRaster = new QgsRasterLayer(currentDirtfw,"basemap");
        QgsProject::instance()->addMapLayer(lRaster);


        lPolygonVector = new QgsVectorLayer("Polygon?crs=EPSG:4326", "polygonLayer", "memory");
        QgsProject::instance()->addMapLayer(lPolygonVector);

        lLineVector = new QgsVectorLayer("LineString?crs=EPSG:4326", "lineLayer", "memory");
        QgsProject::instance()->addMapLayer(lLineVector);

        lPointVector = new QgsVectorLayer("Point?crs=EPSG:4326", "pointLayer", "memory");
        QgsProject::instance()->addMapLayer(lPointVector);

        QgsSymbol *symbol = QgsSymbol::defaultSymbol(lPointVector->geometryType());
        symbol->setColor(QColor("#000000"));
        QgsSingleSymbolRenderer *renderer = new QgsSingleSymbolRenderer(symbol);
        lPointVector->setRenderer(renderer);


        QList <QgsMapLayer *> layers;
        layers.append(lPointVector);
        layers.append(lLineVector);
        layers.append(lPolygonVector);
        layers.append(lRaster);

        mapCanvas->setLayers(layers);
        mapCanvas->setVisible(true);
        mapCanvas->setExtent(QgsRectangle(-127.7,24.4,-79.3,49.1));
       // mapCanvas->zoomToFullExtent();
       // mapCanvas->zoomIn();
        mapCanvas->refresh();

        qDebug()<<"map loading.... end";


    }  catch (std::exception &e) {
        qDebug()<<"Exception : " << e.what();

    }

}

void MainWindow::setupMapTools()
{
    qDebug()<<"setupMapTools.... begin";

    panTool = new PanTool(mapCanvas);
    panTool->setAction(actionPan);

    //pass pointer function to setter.
//    void (MainWindow::*funcptr)(void );
//    funcptr = &MainWindow::onGeometryAdded;

    addPointTool = new AddPointTool(mapCanvas,lPointVector,this);
    addPointTool->setAction(actionEditPoint);

    movePointTool = new MovePointTool(mapCanvas,lPointVector,this);
    movePointTool->setAction(this->actionEdit);

    deletePointTool = new DeleteTool(mapCanvas,lPointVector,this);
    deletePointTool->setAction(this->actionDelete);



    addLineTool = new CaptureTool(mapCanvas,lLineVector,this,CaptureTool::CAPTURE_LINE);
    addLineTool->setAction(this->actionEditLine);

    deleteLineTool = new DeleteTool(mapCanvas,lLineVector,this);
    deleteLineTool->setAction(this->actionDelete);

    editLineTool = new EditTool(mapCanvas,lLineVector,this);
    editLineTool->setAction(this->actionEdit);


    addPolygonTool = new CaptureTool(mapCanvas,lPolygonVector,this,CaptureTool::CAPTURE_POLYGON);
    addPolygonTool->setAction(this->actionEditPolygon);

    deletePolygonTool = new DeleteTool(mapCanvas,lPolygonVector,this);
    deletePolygonTool->setAction(this->actionDelete);

    editPolygonTool = new EditTool(mapCanvas,lPolygonVector,this);
    editPolygonTool->setAction(this->actionEdit);




    qDebug()<<"setupMapTools.... end";
}

void MainWindow::adjustToolbar()
{
    if(currentEditingLayer != 0){
        actionEditPoint->setChecked(currentEditingLayer == lPointVector);
        actionEditPoint->setEnabled(currentEditingLayer == lPointVector);

        actionEditLine->setChecked(currentEditingLayer == lLineVector);
        actionEditLine->setEnabled(currentEditingLayer == lLineVector);

        actionEditPolygon->setChecked(currentEditingLayer == lPolygonVector);
        actionEditPolygon->setEnabled(currentEditingLayer == lPolygonVector);

        actionAdd->setEnabled(true);
        actionEdit->setEnabled(true);
        actionDelete->setEnabled(true);

    }else {

        actionEditPoint->setChecked(false);
        actionEditPoint->setEnabled(true);

        actionEditLine->setChecked(false);
        actionEditLine->setEnabled(true);

        actionEditPolygon->setChecked(false);
        actionEditPolygon->setEnabled(true);

        actionAdd->setChecked(false);
        actionAdd->setEnabled(false);

        actionEdit->setChecked(false);
        actionEdit->setEnabled(false);

        actionDelete->setChecked(false);
        actionDelete->setEnabled(false);

    }
}

void MainWindow::onGeometryAdded()
{
    qDebug()<<"MainWindow - onGeometryAdded.... begin";

      isModified = true;
      mapCanvas->refresh();
      actionAdd->setChecked(false);
      setPanModeSLOT();

      qDebug()<<"MainWindow - onGeometryAdded.... end";
}

void MainWindow::onGeometryChanged()
{
    qDebug()<<"MainWindow - onGeometryChanged.";
    isModified = true;
    mapCanvas->refresh();

}

void MainWindow::onGeometryDeleted()
{
    qDebug()<<"MainWindow - onGeometryDeleted.";
    isModified = true;
    mapCanvas->refresh();
    this->actionDelete->setChecked(false);
    setPanModeSLOT();

}

void MainWindow::quitSLOT()
{
    this->close();
}

void MainWindow::zoomInSLOT()
{
    qDebug("zoomInSLOT");
    mapCanvas->zoomIn();
}

void MainWindow::zoomOutSLOT()
{
    qDebug("zoomOutSLOT");
    mapCanvas->zoomOut();
}

void MainWindow::setPanModeSLOT()
{
    qDebug("setPanModeSLOT");
    actionPan->setChecked(true);
    mapCanvas->setMapTool(panTool);

}

void MainWindow::editPointLayerSLOT()//1
{
    qDebug("editPointLayerSLOT");
    if(lPointVector->isEditable()){//means edit mode is active. so that It mean finishing editing mode...
        if(isModified){
            int reply = QMessageBox::question(this,"Confirm", "Save Changes?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if(reply == QMessageBox::Yes)
                lPointVector->commitChanges();
            else
                lPointVector->rollBack();

        }else {
            lPointVector->commitChanges();
        }
        lPointVector->triggerRepaint();
        currentEditingLayer = 0;
        setPanModeSLOT();

    }else {//start editing mode
        lPointVector->startEditing();
        lPointVector->triggerRepaint();
        this->currentEditingLayer = lPointVector;
        isModified = false;
    }
    adjustToolbar();

}

void MainWindow::editLineLayerSLOT()
{
    qDebug("editLineLayerSLOT");

    if(lLineVector->isEditable()){//means edit mode is active. so that It mean finishing editing mode...
        if(isModified){
            int reply = QMessageBox::question(this,"Confirm", "Save Changes?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if(reply == QMessageBox::Yes)
                lLineVector->commitChanges();
            else
                lLineVector->rollBack();

        }else {
            lLineVector->commitChanges();
        }
        lLineVector->triggerRepaint();
        currentEditingLayer = 0;
        setPanModeSLOT();

    }else {//start editing mode
        lLineVector->startEditing();
        lLineVector->triggerRepaint();
        this->currentEditingLayer = lLineVector;
        isModified = false;
    }
    adjustToolbar();

}

void MainWindow::editPolygonLayerSLOT()
{
    qDebug("editPolygonLayerSLOT");

    if(lPolygonVector->isEditable()){//means edit mode is active. so that It mean finishing editing mode...
        if(isModified){
            int reply = QMessageBox::question(this,"Confirm", "Save Changes?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if(reply == QMessageBox::Yes)
                lPolygonVector->commitChanges();
            else
                lPolygonVector->rollBack();

        }else {
            lPolygonVector->commitChanges();
        }
        lPolygonVector->triggerRepaint();
        currentEditingLayer = 0;
        setPanModeSLOT();

    }else {//start editing mode
        lPolygonVector->startEditing();
        lPolygonVector->triggerRepaint();
        this->currentEditingLayer = lPolygonVector;
        isModified = false;
    }
    adjustToolbar();


}

void MainWindow::addFeatureSLOT()//2
{
    qDebug("addFeatureSLOT");

    if(!actionAdd->isChecked()){
        setPanModeSLOT();
        return;
    }

    if(currentEditingLayer == lPointVector){
        mapCanvas->setMapTool(addPointTool);
    }else if (currentEditingLayer == lLineVector) {
        mapCanvas->setMapTool(addLineTool);
    }else if (currentEditingLayer == lPolygonVector) {
        mapCanvas->setMapTool(addPolygonTool);
    }



}

void MainWindow::editFeatureSLOT()
{
    qDebug("editFeatureSLOT");

    if(!actionEdit->isChecked()){
        setPanModeSLOT();
        return;
    }

    if(currentEditingLayer == lPointVector){
        mapCanvas->setMapTool(movePointTool);
    }else if (currentEditingLayer == lLineVector) {
        // TODO
        mapCanvas->setMapTool(editLineTool);
    }else if (currentEditingLayer == lPolygonVector) {
        // TODO
        qDebug("editFeatureSLOT -currentEditingLayer is  lPolygonVector");

        mapCanvas->setMapTool(editPolygonTool);
    }



}

void MainWindow::deleteFeatureSLOT()
{
    qDebug("deleteFeatureSLOT");

    if(!actionDelete->isChecked()){
        setPanModeSLOT();
        return;
    }

    if(currentEditingLayer == lPointVector){
        mapCanvas->setMapTool(deletePointTool);
    }else if (currentEditingLayer == lLineVector) {
        // TODO
        mapCanvas->setMapTool(deleteLineTool);

    }else if (currentEditingLayer == lPolygonVector) {
        // TODO
        mapCanvas->setMapTool(deletePolygonTool);

    }

}



AddPointTool::AddPointTool(QgsMapCanvas *canvas, QgsVectorLayer *layer, ICallBack *callback): QgsMapTool(canvas)
{
    this->canvas = canvas;
    this->pLayer = layer;
    this->callbackFunc = callback;
}

void AddPointTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{
    qDebug()<<"AddPointTool::canvasReleaseEvent ==> e->pos()| x:  " <<e->pos().x() << " | y: " <<e->pos().y();

    QgsPointXY point = this->toLayerCoordinates(pLayer,e->pos());

    qDebug()<<"AddPointTool::canvasReleaseEvent ==> toLayerCoordinates| point pos for layer (lat long):  " <<point.toString();

    QgsFeature feature;
    feature.setGeometry(QgsGeometry::fromPointXY(point));
    pLayer->addFeature(feature);
    pLayer->updateExtents();
    callbackFunc->onGeometryAdded();

}




MovePointTool::MovePointTool(QgsMapCanvas *canvas, QgsVectorLayer *layer, ICallBack *callback) : QgsMapToolIdentify(canvas)
{
    qDebug()<<"MovePointTool--construtor.";
    this->canvas = canvas;
    setCursor(Qt::CrossCursor);
    pLayer=layer;
    callbackFunc = callback;


}

void MovePointTool::canvasMoveEvent(QgsMapMouseEvent *e)
{
    qDebug()<<"MovePointTool--canvasMoveEvent.";

    if(dragging){
        QgsPointXY point = this->toLayerCoordinates(pLayer,e->pos());
        QgsGeometry geometry = QgsGeometry::fromPointXY(point);
        pLayer->changeGeometry(this->feature.id(),geometry);
        canvas->refresh();
        moved = true;
    }


}

void MovePointTool::canvasPressEvent(QgsMapMouseEvent *e)
{
    qDebug()<<"MovePointTool--canvasPressEvent.";
     QList<QgsMapLayer *> layerList;
     layerList.append(pLayer);
    QList<QgsMapToolIdentify::IdentifyResult> found_features =  this->identify(e->x(),e->y(),layerList,TopDownAll);
    if(found_features.size() > 0) {
        dragging = true;
        feature = found_features.at(0).mFeature;
        moved = true;

    }else {
        dragging = false;
        feature = NULL;
    }

}

void MovePointTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{
    qDebug()<<"MovePointTool--canvasReleaseEvent.";
    dragging = false;
    feature = NULL;

    if(moved){
        callbackFunc->onGeometryChanged();
        moved=false;
    }


}

DeleteTool::DeleteTool(QgsMapCanvas *canvas, QgsVectorLayer *layer, ICallBack *callback): QgsMapToolIdentify(canvas)
{
    qDebug()<<"DeleteTool--construtor.";

    this->canvas = canvas;
    setCursor(Qt::CrossCursor);
    pLayer=layer;
    callbackFunc = callback;
    feature = NULL;


}

void DeleteTool::canvasPressEvent(QgsMapMouseEvent *e)
{
    QList<QgsMapLayer *> layerList;
    layerList.append(pLayer);
    QList<QgsMapToolIdentify::IdentifyResult> found_features =  this->identify(e->x(),e->y(),layerList,TopDownAll);
    if(found_features.size() > 0) {
        feature = found_features.at(0).mFeature;
    }else
        feature = NULL;


}

void DeleteTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{
    QList<QgsMapLayer *> layerList;
    layerList.append(pLayer);
    QList<QgsMapToolIdentify::IdentifyResult> found_features =  this->identify(e->x(),e->y(),layerList,TopDownAll);
    if(found_features.size() > 0) {
        if(feature.id() == found_features.at(0).mFeature.id()){
            pLayer->deleteFeature(feature.id());
            callbackFunc->onGeometryDeleted();
        }
    }

}


CaptureTool::CaptureTool(QgsMapCanvas *canvas, QgsVectorLayer *layer, ICallBack *callback, CaptureMode captureMode): QgsMapTool(canvas)
{
    this->canvas = canvas;
    this->pLayer = layer;
    this->callbackFunc = callback;
    this->pCaptureMode = captureMode;
    this->rubberBand = NULL;
    this->tempRubberBand = NULL;
    this->capturing = false;


}

void CaptureTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{
    if(e->button()== Qt::LeftButton){
       if(!capturing){
            startCapturing();
       }
       addVertex(e->pos());
    }else if(e->button()== Qt::RightButton){
       QList<CaptureTool::CapturedPoints> points =  getCapturedGeometry();
        stopCapturing();
        if (points.size() > 0){
            geometryCaptured(points);
        }
    }
}

void CaptureTool::canvasMoveEvent(QgsMapMouseEvent *e)
{
    qDebug()<<"CaptureTool -- canvasMoveEvent";
    if (this->tempRubberBand  && capturing){
       CapturedPoints point =  transformCoordinates(e->pos());
       tempRubberBand->movePoint(point.toMapCoordinatePoint);
    }

}

void CaptureTool::keyPressEvent(QKeyEvent *e)
{
    qDebug()<<"keyPressEvent";

    if(e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete){
        removeLastVertex();
        e->ignore();
    }

    if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter){
         QList<CapturedPoints> points =   getCapturedGeometry();
         stopCapturing();
         if(points.size() > 0){
             geometryCaptured(points);
         }
    }


}

void CaptureTool::startCapturing()
{
    QColor red("red");
    red.setAlphaF(0.78);

    if(pCaptureMode == CaptureMode::CAPTURE_LINE){
        this->rubberBand = new QgsRubberBand(canvas,QgsWkbTypes::LineGeometry);
        this->tempRubberBand = new QgsRubberBand(canvas,QgsWkbTypes::LineGeometry);

    }else {
        this->rubberBand = new QgsRubberBand(canvas,QgsWkbTypes::PolygonGeometry);
        this->tempRubberBand = new QgsRubberBand(canvas,QgsWkbTypes::PolygonGeometry);
    }

    rubberBand->setWidth(2);
    rubberBand->setColor(red);
    rubberBand->show();



    tempRubberBand->setWidth(2);
    tempRubberBand->setColor(red);
    tempRubberBand->setLineStyle(Qt::DotLine);
    tempRubberBand->show();

    capturing = true;

}

void CaptureTool::stopCapturing()
{
    if(this->rubberBand != NULL){
        canvas->scene()->removeItem(this->rubberBand);
        delete rubberBand;
        rubberBand = NULL;
    }

    if(this->tempRubberBand  != NULL){
        canvas->scene()->removeItem(this->tempRubberBand);
        delete tempRubberBand;
        tempRubberBand = NULL;
    }
    capturing = false;
    capturedPoints.clear();
    canvas->refresh();


}

void CaptureTool::addVertex(QPoint canvasPoint)
{
    CapturedPoints transformedPoints = transformCoordinates(canvasPoint);
    rubberBand->addPoint(transformedPoints.toMapCoordinatePoint);
    capturedPoints.append(transformedPoints);//layer + map point

    if(pCaptureMode == CaptureMode::CAPTURE_LINE){
        tempRubberBand->reset(QgsWkbTypes::LineGeometry);
        tempRubberBand->addPoint(transformedPoints.toMapCoordinatePoint);
    }else {
        tempRubberBand->reset(QgsWkbTypes::PolygonGeometry);
        const QgsPointXY *firstPoint = rubberBand->getPoint(0,0);
        tempRubberBand->addPoint(*firstPoint);
        tempRubberBand->movePoint(transformedPoints.toMapCoordinatePoint);
        tempRubberBand->addPoint(transformedPoints.toMapCoordinatePoint);

    }



}

QList<CaptureTool::CapturedPoints> CaptureTool::getCapturedGeometry()
{
    QList<CapturedPoints> list;
//    return list;

    if(pCaptureMode == CAPTURE_LINE && capturedPoints.size() < 2)
        return list;


    if(pCaptureMode == CAPTURE_POLYGON && capturedPoints.size() < 3)
        return list;

    if(pCaptureMode == CAPTURE_POLYGON )
        capturedPoints.append(capturedPoints.at(0));//close polygon
   return  capturedPoints;
}

void CaptureTool::geometryCaptured(QList<CaptureTool::CapturedPoints> points)
{
    qDebug()<<"CaptureTool::geometryCaptured-- begin |  pointSize : "<<points.size();
    //QVector< QgsPoint >
    QgsGeometry geometry ;
     if(pCaptureMode == CAPTURE_LINE){
         qDebug()<<"CaptureTool::geometryCaptured CAPTURE_LINE ";

         QgsPolylineXY vectorPolyLine;
         for(auto point : points){
             vectorPolyLine.append(point.toLayerCoordinatePoint);
         }
       geometry =  QgsGeometry::fromPolylineXY(vectorPolyLine);
     }else if(pCaptureMode == CAPTURE_POLYGON){
         qDebug()<<"CaptureTool::geometryCaptured CAPTURE_POLYGON ";

         QgsPolygonXY vectorPolygon;
         QgsPolylineXY vectorPolyLine;

         for(auto point : points){
             vectorPolyLine.append(point.toLayerCoordinatePoint);
         }
         vectorPolygon.append(vectorPolyLine);

        geometry =  QgsGeometry::fromPolygonXY(vectorPolygon);

     }
    QgsFeature feature;
    feature.setGeometry(geometry);
    pLayer->addFeature(feature);
    pLayer->updateExtents();
    callbackFunc->onGeometryAdded();
    qDebug()<<"CaptureTool::geometryCaptured end ";

}


CaptureTool::CapturedPoints CaptureTool::transformCoordinates(QPoint canvasPoint)
{
       QgsPointXY mapPoint =  this->toMapCoordinates(canvasPoint);
       QgsPointXY layerPoint = this->toLayerCoordinates(pLayer , canvasPoint);

       CapturedPoints retPoints;
       retPoints.toLayerCoordinatePoint = layerPoint;
       retPoints.toMapCoordinatePoint = mapPoint;

       return retPoints;
}

void CaptureTool::removeLastVertex()
{
    if (capturing == false)
        return;

    int bandSize = rubberBand->numberOfVertices();
    int tempBandSize = tempRubberBand->numberOfVertices();
    int numPoints = capturedPoints.size();


    if(bandSize < 1 || numPoints < 1)
        return;

    this->rubberBand->removePoint(-1);

    if(bandSize > 1){
        if(tempBandSize > 1){
            const QgsPointXY *point =     rubberBand->getPoint(0,bandSize-2);
            tempRubberBand->movePoint(tempBandSize-2,*point);
        }else {
            if(pCaptureMode == CAPTURE_LINE){
                tempRubberBand->reset(QgsWkbTypes::LineGeometry);
            }else if(pCaptureMode == CAPTURE_POLYGON){
                tempRubberBand->reset(QgsWkbTypes::PolygonGeometry);
            }
        }
    }
    capturedPoints.removeLast();


}


EditTool::EditTool(QgsMapCanvas *canvas, QgsVectorLayer *layer, ICallBack *callback): QgsMapTool(canvas)
{
    setCursor(Qt::CrossCursor);
    this->canvas = canvas;
    pLayer = layer;
    callbackFunc = callback;
    dragging = false;

}

void EditTool::canvasMoveEvent(QgsMapMouseEvent *e)
{
    qDebug()<<"EditTool::canvasMoveEvent ";


    if(dragging){
        moveVertexTo(e->pos());
        canvas->refresh();
    }

}

void EditTool::canvasDoubleClickEvent(QgsMapMouseEvent *e)
{
    qDebug()<<"EditTool::canvasDoubleClickEvent -begin ";

    QgsFeature feature = this->findFeatureAt(e->pos());
    if(feature == NULL){
        qDebug()<<"EditTool::canvasDoubleClickEvent -findFeature is NULL ";

        return;
    }
     CapturedPoints point = transformCoordinates(e->pos());
     QgsGeometry geometry = feature.geometry();


    QgsPointXY closestPt;
    int beforeVertexIndex ;

     double distSquared =
                 geometry.closestSegmentWithContext(point.toLayerCoordinatePoint,closestPt,beforeVertexIndex);

     double distance = qSqrt(distSquared);
     int tolerance = calculateTolerance(e->pos());

     if(distance > tolerance){
         qDebug()<<"EditTool::canvasDoubleClickEvent -distance > tolerance so that return ";

         return;
     }
    geometry.insertVertex(closestPt.x(),closestPt.y(),beforeVertexIndex);
    pLayer->changeGeometry(feature.id(),geometry);
    canvas->refresh();
    qDebug()<<"EditTool::canvasDoubleClickEvent -end ";

}

void EditTool::canvasPressEvent(QgsMapMouseEvent *e)
{
    qDebug()<<"EditTool::canvasPressEvent -begin ";


    QgsFeature feature = this->findFeatureAt(e->pos());
    if(feature == NULL){
        qDebug()<<"EditTool::canvasPressEvent -findFeature is NULL ";

        return;
    }

    CapturedPoints point = transformCoordinates(e->pos());
    QgsGeometry geometry = feature.geometry();

    int closestVertexIndex;
    int previousVertexIndex;
    int nextVertexIndex;
    double sqrDist;


    QgsPointXY vertexCoor =
                geometry.closestVertex(point.toLayerCoordinatePoint,closestVertexIndex,previousVertexIndex,nextVertexIndex,sqrDist);

    double distance = qSqrt(sqrDist);
    int tolerance = calculateTolerance(e->pos());

    if(distance > tolerance){
        qDebug()<<"EditTool::canvasPressEvent -distance > tolerance so that return ";

        return;
    }

    if(e->button() == Qt::LeftButton){

        dragging = true;
        this->feature = feature;
        this->vertex = closestVertexIndex;
        this->moveVertexTo(e->pos());
        canvas->refresh();

    }else if (e->button() == Qt::RightButton){
        this->deleteVertex(this->feature,vertex);
        canvas->refresh();
    }

    qDebug()<<"EditTool::canvasPressEvent -end ";

}

void EditTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{
    qDebug()<<"EditTool::canvasReleaseEvent -begin ";

    if (dragging){
        moveVertexTo(e->pos());
        pLayer->updateExtents();
        canvas->refresh();
        dragging = false;
        feature  = NULL;
        vertex   = -1000;
    }
    qDebug()<<"EditTool::canvasReleaseEvent -end ";

}

EditTool::CapturedPoints EditTool::transformCoordinates(QPoint canvasPoint)
{
    QgsPointXY mapPoint =  this->toMapCoordinates(canvasPoint);
    QgsPointXY layerPoint = this->toLayerCoordinates(pLayer , canvasPoint);

    CapturedPoints retPoints;
    retPoints.toLayerCoordinatePoint = layerPoint;
    retPoints.toMapCoordinatePoint = mapPoint;

    return retPoints;
}

double EditTool::calculateTolerance(QPoint pos)
{
   /*Calculate the "tolerance" to use for a mouse-click.

        'pos' is a QPoint object representing the clicked-on point, in
        canvas coordinates.

        The tolerance is the number of map units away from the click
        position that a vertex or geometry can be and we still consider it
        to be a click on that vertex or geometry.
    */

    QPoint pt1 = QPoint(pos.x(), pos.y());
    QPoint pt2 = QPoint(pos.x() + 10, pos.y());

    CapturedPoints capturedPt1 = transformCoordinates(pt1);
    CapturedPoints capturedPt2 = transformCoordinates(pt2);
    double tolerance = capturedPt2.toLayerCoordinatePoint.x() - capturedPt1.toLayerCoordinatePoint.x();
    return tolerance;
}

QgsFeature EditTool::findFeatureAt(QPoint pos)
{
    /* Return the feature at the given position.

        'pos' is in canvas coordinates.

        If no feature is close to the given coordinate, we return None.
    */

    CapturedPoints point = transformCoordinates(pos);
    double tolerance = calculateTolerance(pos);

    QgsRectangle searchRect = QgsRectangle(point.toLayerCoordinatePoint.x() - tolerance,
                                           point.toLayerCoordinatePoint.y() - tolerance,
                                           point.toLayerCoordinatePoint.x() + tolerance,
                                           point.toLayerCoordinatePoint.y() + tolerance);

    QgsFeatureRequest request;
    request.setFilterRect(searchRect);
    request.setFlags(QgsFeatureRequest::ExactIntersect);
   QgsFeatureIterator featureIterator =  pLayer->getFeatures(request);
   QgsFeature retFeature;
    bool ret = featureIterator.nextFeature(retFeature);
    if(ret)
        return retFeature;
    else
        return NULL;


}

void EditTool::moveVertexTo(QPoint pos)
{

    /* Move the edited vertex to the given position.

        'pos' is in canvas coordinates.
    */

    QgsGeometry  geometry = this->feature.geometry();
    QgsPointXY layerPoint = toLayerCoordinates(pLayer,pos);
    geometry.moveVertex(layerPoint.x(),layerPoint.y(),this->vertex);
    this->pLayer->changeGeometry(this->feature.id(),geometry);
    this->callbackFunc->onGeometryChanged();

}

void EditTool::deleteVertex(QgsFeature feature, int vertex)
{
    QgsGeometry geometry = feature.geometry();

    if(geometry.wkbType() == QgsWkbTypes::LineString){
        QgsPolylineXY lineString = geometry.asPolyline();
        if(lineString.size() <=2){
            return;
        }

    }else if(geometry.wkbType() == QgsWkbTypes::Polygon){
        QgsPolygonXY polygon = geometry.asPolygon();
        const QVector<QgsPointXY> points = polygon.at(0);
        if(points.size() <= 4)// first point == last point
            return;
    }

    if(geometry.deleteVertex(vertex)){
        pLayer->changeGeometry(feature.id(),geometry);
        this->callbackFunc->onGeometryChanged();
    }

}
