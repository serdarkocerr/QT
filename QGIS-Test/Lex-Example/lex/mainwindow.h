#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <QMainWindow>

#include <qgsapplication.h>
#include <qgsproviderregistry.h>

#include <qgssinglesymbolrenderer.h>
#include <qgsmaplayer.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgsproject.h>
#include <qgssymbol.h>
#include <QVBoxLayout>
#include <qgsrulebasedrenderer.h>
#include <qgsmaptool.h>
#include <qgsmapmouseevent.h>
#include <qgsmaptoolidentify.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PanTool : public QgsMapTool {

public:

   PanTool(QgsMapCanvas *canvas);



   // QgsMapTool interface
public:
   void canvasMoveEvent(QgsMapMouseEvent *e);
   void canvasReleaseEvent(QgsMapMouseEvent *e);
};

class ExploreTool : public QgsMapToolIdentify{
public:

    ExploreTool(QgsMapCanvas *canvas);



    // QgsMapTool interface
public:
    void canvasReleaseEvent(QgsMapMouseEvent *e);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadMap();
    void arrangeLandMarks(QgsMapLayer *mapLayer,QgsRuleBasedRenderer *renderer,QgsSymbol *symbol);
    void createAndConnectToolbar();
    void setPanMode();
    void setExploreMode();
public slots:
    void showBaseMapLayerSLOT();
    void showLandMapLayerSLOT();
    void zoomInSLOT();
    void zoomOutSLOT();
    void setPanModeSLOT();
    void setExploreModeSLOT();

private:
    QgsMapCanvas* mapCanvas;
    QVBoxLayout *playout;
    PanTool *panTool;
    ExploreTool *exploreTool;
    QAction *actionPan;
    QAction *actionExplore;
    QAction *actionShowBaseMapLayer;
    QAction *actionShowLandmarkLayer;
    QgsVectorLayer *lVector;
    QgsRasterLayer *lRaster;
    Ui::MainWindow *ui;

};


#endif // MAINWINDOW_H
