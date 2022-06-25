#include "mainwindow.h"
#include <qgis_core.h>
#include <qgis_gui.h>
#include <qgis.h>
#include "ui_mainwindow.h"
#include <QString>
#include <qgsrasterlayer.h>
#include <QDebug>
#include <qgspallabeling.h>
#include <qgsvectorlayerlabeling.h>
#include <qgspropertycollection.h>
#include <qgssymbol.h>
#include <iostream>
#include <qgsrulebasedlabeling.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <qmessagebox.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setMinimumSize(800,400);
    /** UI Design */
    this->setWindowTitle("Landmark Explorer");
    createAndConnectToolbar();


    mapCanvas = new QgsMapCanvas();
    mapCanvas->setCanvasColor(Qt::white);

    playout  = new QVBoxLayout;
    playout->setContentsMargins(0,0,0,0);
    playout->addWidget(mapCanvas);

    centralWidget()->setLayout(playout);
//    loadMap();

    panTool = new PanTool(mapCanvas);
    panTool->setAction(actionPan);

    exploreTool = new ExploreTool(mapCanvas);
    exploreTool->setAction(actionExplore);

    actionShowBaseMapLayer->setChecked(true);
    actionShowLandmarkLayer->setChecked(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMap()
{
    try {
        qDebug()<<"map loading.... begin";
    //    QString currentDir = QDir::currentPath();
    //    QString filename = currentDir + QString("");
        QString currentDirtfw="D:/Qt5.12.12/workspace/Lex-Example/data/NE1_HR_LC_SR_W_DR.tif";
        lRaster = new QgsRasterLayer(currentDirtfw,"basemap");
        QgsProject::instance()->addMapLayer(lRaster);

        QString currentDirShp ="D:/Qt5.12.12/workspace/Lex-Example/data/ne_10m_populated_places.shp";
        lVector = new QgsVectorLayer(currentDirShp,"landmarks","ogr");
        QgsProject::instance()->addMapLayer(lVector);

//        /** labels are unreadable so that some label( landmark labels) will be shown due to zooms levels.*/
//        QgsSymbol *symbol = QgsSymbol::defaultSymbol(lVector->geometryType());
//        QgsRuleBasedRenderer *renderer = new QgsRuleBasedRenderer(symbol);
//        arrangeLandMarks(lVector,renderer,symbol);
//        renderer->rootRule()->removeChildAt(0);
//        lVector->setRenderer(renderer);
//       // lVector->triggerRepaint();
//        /** */

//        /** Location Label visible-begin */
//        QgsPalLayerSettings label_settings;
//        label_settings.fieldName="NAME";
//        label_settings.displayAll=true;
//        label_settings.drawLabels=true;
//        label_settings.placement = QgsPalLayerSettings::OverPoint;
//        label_settings.quadOffset = QgsPalLayerSettings::QuadrantBelow;
//        label_settings.yOffset = 1;
//        label_settings.isExpression=true;

//        QgsVectorLayerSimpleLabeling *lab = new QgsVectorLayerSimpleLabeling(label_settings);
//        lVector->setLabeling(lab);
//        lVector->setLabelsEnabled(true);
//        lVector->triggerRepaint();
//        /** Location Label visible-end  */


        QgsPalLayerSettings *label_settings = new QgsPalLayerSettings();
        label_settings->fieldName="NAME";
        label_settings->displayAll=true;
        label_settings->drawLabels=true;
        label_settings->placement = QgsPalLayerSettings::OverPoint;
        label_settings->quadOffset = QgsPalLayerSettings::QuadrantBelow;
        label_settings->yOffset = 1;
        label_settings->isExpression=true;
//        std::string exp = std::string("('CASE WHEN SCALERANK IN (0,1) THEN 18 '") +
//                std::string("'WHEN SCALERANK IN (2, 3, 4) THEN 14 '") +
//                std::string("'WHEN SCALERANK IN (5, 6, 7) THEN 12 '") +
//                std::string("'WHEN SCALERANK IN (8, 9, 10) THEN 10 '") +
//                std::string("'ELSE 9 END')");

//        QgsProperty prop = QgsProperty::fromExpression(QString::fromStdString(exp),true);

//        prop.setActive(true);
//        QgsPropertyCollection propcol;
//        propcol.setProperty(1,prop);
//        label_settings->setDataDefinedProperties(propcol);

        /** create rules*/
      // QgsRuleBasedLabeling::Rule root_rule =  QgsRuleBasedLabeling::Rule();
        QgsRuleBasedLabeling::Rule *root=  new QgsRuleBasedLabeling::Rule( new QgsPalLayerSettings());
        QgsRuleBasedLabeling::Rule *rule1=  new QgsRuleBasedLabeling::Rule( label_settings);
        rule1->setFilterExpression("(SCALERANK >=0) AND (SCALERANK <=1)");
        rule1->setMinimumScale(99999999);//zoom out
        rule1->setMaximumScale(100);//zoom in
        root->appendChild(rule1);

        QgsRuleBasedLabeling::Rule *rule2=  new QgsRuleBasedLabeling::Rule( label_settings);
        rule2->setFilterExpression("(SCALERANK >=2) AND (SCALERANK <=4)");
        rule2->setMinimumScale(10000000);
        rule2->setMaximumScale(0);
        root->appendChild(rule2);

        QgsRuleBasedLabeling::Rule *rule3=  new QgsRuleBasedLabeling::Rule( label_settings);
        rule3->setFilterExpression("(SCALERANK >=5) AND (SCALERANK <=7)");
        rule3->setMinimumScale(5000000);
        rule3->setMaximumScale(0);
        root->appendChild(rule3);

        QgsRuleBasedLabeling::Rule *rule4=  new QgsRuleBasedLabeling::Rule( label_settings);
        rule4->setFilterExpression("(SCALERANK >=8) AND (SCALERANK <=10)");
        rule4->setMinimumScale(2000000);
        rule4->setMaximumScale(0);
        root->appendChild(rule4);

        lVector->setLabelsEnabled(true);

        QgsRuleBasedLabeling *rules = new QgsRuleBasedLabeling(root);

        lVector->setLabeling(rules);

        QList <QgsMapLayer *> layers;
        layers.append(lVector);
        layers.append(lRaster);

        mapCanvas->setLayers(layers);
        mapCanvas->setVisible(true);
        mapCanvas->setExtent(QgsRectangle(-127.7,24.4,-79.3,49.1));
        mapCanvas->refresh();

        qDebug()<<"map loading.... end";
    }  catch ( std::exception &e) {
            qDebug(e.what());
    }

}

void MainWindow::arrangeLandMarks(QgsMapLayer *mapLayer,QgsRuleBasedRenderer *renderer,QgsSymbol *symbol)
{
    QgsVectorLayer* layer = static_cast<QgsVectorLayer*>(mapLayer);
    qDebug("arrangeLandMarks -begin");
    if(layer){
        qDebug("layer is  QgsVectorLayer");
//        QgsSymbol *symbol = QgsSymbol::defaultSymbol(layer->geometryType());
//        QgsRuleBasedRenderer *renderer = new QgsRuleBasedRenderer(symbol);

        QgsRuleBasedRenderer::Rule *root_rule = renderer->rootRule();
        QgsRuleBasedRenderer::Rule *default_rule = root_rule->children().at(0);


        QgsRuleBasedRenderer::Rule *rule1 = default_rule->clone();
        QString filter1 = QString::fromStdString("(SCALERANK >=0) AND (SCALERANK <=1)");
//        rule1->setFilterExpression('(SCALERANK >=0) AND (SCALERANK <=1)');
        rule1->setFilterExpression(filter1);
        rule1->setMinimumScale(0);
        rule1->setMaximumScale(99999999);
        root_rule->appendChild(rule1);


        QgsRuleBasedRenderer::Rule *rule2 = default_rule->clone();
        QString filter2 = QString::fromStdString("(SCALERANK >=2) AND (SCALERANK <=4)");
//        rule2->setFilterExpression("(SCALERANK  >=2) AND (SCALERANK <=4)");
        rule2->setFilterExpression(filter2);
        rule2->setMinimumScale(0);
        rule2->setMaximumScale(10000000);
        root_rule->appendChild(rule2);

        QgsRuleBasedRenderer::Rule *rule3 = default_rule->clone();
//        rule3->setFilterExpression("(SCALERANK >=5) AND (SCALERANK <=7)");
        QString filter3 = QString::fromStdString("(SCALERANK >=5) AND (SCALERANK <=7)");
        rule3->setFilterExpression(filter3);
        rule3->setMinimumScale(0);
        rule3->setMaximumScale(5000000);
        root_rule->appendChild(rule3);

        QgsRuleBasedRenderer::Rule *rule4 = default_rule->clone();
//        rule4->setFilterExpression("(SCALERANK >=7) AND (SCALERANK <=10)");
        QString filter4 = QString::fromStdString("(SCALERANK >=7) AND (SCALERANK <=10)");
        rule4->setFilterExpression(filter4);

        rule4->setMinimumScale(0);
        rule4->setMaximumScale(2000000);
        root_rule->appendChild(rule4);

//        root_rule->removeChildAt(0);
//        layer->setRenderer(renderer);
        //layer->triggerRepaint();

        qDebug("arrangeLandMarks -end");

    }else{
        //TODO
        qDebug("TODO");
    }


}

void MainWindow::createAndConnectToolbar()
{
    QMenu *fileMenu = this->menuBar()->addMenu("File");
    QMenu *viewMenu = this->menuBar()->addMenu("View");
    QMenu *modeMenu = this->menuBar()->addMenu("Mode");

    QToolBar *toolbar  = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea, toolbar);


    QAction *actionQuit = new QAction("Quit",this);
    actionQuit->setShortcut(QKeySequence::Quit);

    actionShowBaseMapLayer = new QAction("Basemap",this);
    actionShowBaseMapLayer->setShortcut(QKeySequence("Ctrl+B"));
    actionShowBaseMapLayer->setCheckable(true);

    actionShowLandmarkLayer = new QAction("Landmarks",this);
    actionShowLandmarkLayer->setShortcut(QKeySequence("Ctrl+L"));
    actionShowLandmarkLayer->setCheckable(true);

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

    QIcon iconExplore(":/icon/mActionExplore.png");
    actionExplore = new QAction(iconExplore,"Explore",this);
    actionExplore->setShortcut(QKeySequence("Ctrl+2"));
    actionExplore->setCheckable(true);


    fileMenu->addAction(actionQuit);

    viewMenu->addAction(actionShowBaseMapLayer);
    viewMenu->addAction(actionShowLandmarkLayer);
    viewMenu->addSeparator();
    viewMenu->addAction(actionZoomIn);
    viewMenu->addAction(actionZoomOut);

    modeMenu->addAction(actionPan);
    modeMenu->addAction(actionExplore);

    toolbar->addAction(actionZoomIn);
    toolbar->addAction(actionZoomOut);
    toolbar->addAction(actionPan);
    toolbar->addAction(actionExplore);

    this->resize(this->sizeHint());


    connect(actionQuit,SIGNAL(triggered()),this,SLOT(quit()));
    connect(actionShowBaseMapLayer,SIGNAL(triggered()),this,SLOT(showBaseMapLayerSLOT()));
    connect(actionShowLandmarkLayer,SIGNAL(triggered()),this,SLOT(showLandMapLayerSLOT()));
    connect(actionZoomIn,SIGNAL(triggered()),this,SLOT(zoomInSLOT()));
    connect(actionZoomOut,SIGNAL(triggered()),this,SLOT(zoomOutSLOT()));
    connect(actionPan,SIGNAL(triggered()),this,SLOT(setPanModeSLOT()));
    connect(actionExplore,SIGNAL(triggered()),this,SLOT(setExploreModeSLOT()));
}

void MainWindow::setPanMode()
{
    actionPan->setChecked(true);
    actionExplore->setChecked(false);

    mapCanvas->setMapTool(panTool);
}

void MainWindow::setExploreMode()
{
    actionPan->setChecked(false);
    actionExplore->setChecked(true);
    mapCanvas->setMapTool(exploreTool);
}

void MainWindow::showBaseMapLayerSLOT()
{
     qDebug("showBaseMapLayerSLOT");

     QList <QgsMapLayer *> layers;
     if(actionShowLandmarkLayer->isChecked())
         layers.append(lVector);
     if(actionShowBaseMapLayer->isChecked())
        layers.append(lRaster);


     mapCanvas->setLayers(layers);
     mapCanvas->setVisible(true);
     mapCanvas->refresh();


}

void MainWindow::showLandMapLayerSLOT()
{
    qDebug("showLandMapLayerSLOT");

    QList <QgsMapLayer *> layers;
    if(actionShowLandmarkLayer->isChecked())
        layers.append(lVector);
    if(actionShowBaseMapLayer->isChecked())
       layers.append(lRaster);


    mapCanvas->setLayers(layers);
    mapCanvas->setVisible(true);
    mapCanvas->refresh();



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
    setPanMode();

}

void MainWindow::setExploreModeSLOT()
{
    qDebug("setExploreModeSLOT");
    setExploreMode();
}


PanTool::PanTool(QgsMapCanvas *canvas) : QgsMapTool(canvas)
{
    this->setCursor(Qt::OpenHandCursor);

}

void PanTool::canvasMoveEvent(QgsMapMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        //dragging false  ?
        this->canvas()->panAction(e);
    }
}

void PanTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        this->canvas()->panActionEnd(e->pos());

}

// display to feature when user clicked to landmark
ExploreTool::ExploreTool(QgsMapCanvas *canvas) : QgsMapToolIdentify(canvas)
{

}

void ExploreTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{
    QList<QgsMapToolIdentify::IdentifyResult> found_features =
            this->identify(e->x(), e->y() , TopDownStopAtFirst, VectorLayer);

    QList<QString> info;

    if(found_features.size() > 0){
        QgsMapLayer *layer = found_features.at(0).mLayer;
        QgsFeature feature = found_features.at(0).mFeature;
        QgsGeometry geometry = feature.geometry();

        QString name = feature.attribute("NAME").toString();
        if(!feature.attribute("NAME").isNull())
            info.append(name);

        QString admin_0 = feature.attribute("ADM0NAME").toString();
        QString admin_1 = feature.attribute("ADM1NAME").toString();

        if(!admin_0.isEmpty() && !admin_1.isEmpty()){
            info.append(admin_1 + ", " + admin_0);
        }

        QString timezone = feature.attribute("TIMEZONE").toString();

        if(!feature.attribute("TIMEZONE").isNull())
            info.append("Timezone : " + timezone);

        double longitude = geometry.asPoint().x();
        double latitude = geometry.asPoint().y();

        info.append("Lat/Long " + QString::number(longitude,'f',4) +
                    " , " + QString::number(latitude,'f',4));

        QString infos;
        for( QString str:info){
            infos.append(str);
        }
        QMessageBox::information(canvas(),"Feature Info ", infos);

    }
}
