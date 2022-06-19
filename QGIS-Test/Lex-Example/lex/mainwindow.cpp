#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <qgsrasterlayer.h>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setMinimumSize(800,400);
    mapCanvas = new QgsMapCanvas();
    mapCanvas->setCanvasColor(Qt::white);

    playout  = new QVBoxLayout;
    playout->setContentsMargins(0,0,0,0);
    playout->addWidget(mapCanvas);

    centralWidget()->setLayout(playout);
    loadMap();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMap()
{
    qDebug()<<"map loading.... begin";
//    QString currentDir = QDir::currentPath();
//    QString filename = currentDir + QString("");
    QString currentDirtfw="D:/Qt5.12.12/workspace/Lex-Example/data/NE1_HR_LC_SR_W_DR.tif";
    QgsRasterLayer *ll = new QgsRasterLayer(currentDirtfw,"basemap");
    QgsProject::instance()->addMapLayer(ll);

    QString currentDirShp ="D:/Qt5.12.12/workspace/Lex-Example/data/ne_10m_populated_places.shp";
   QgsVectorLayer *ll2 = new QgsVectorLayer(currentDirShp,"landmarks","ogr");
   QgsProject::instance()->addMapLayer(ll2);
   QList <QgsMapLayer *> layers;
   layers.append(ll2);
   layers.append(ll);

   mapCanvas->setLayers(layers);
   mapCanvas->setVisible(true);
   mapCanvas->setExtent(QgsRectangle(-127.7,24.4,-79.3,49.1));
   mapCanvas->refresh();

qDebug()<<"map loading.... end";




}

