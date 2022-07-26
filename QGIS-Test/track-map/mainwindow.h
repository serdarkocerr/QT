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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadMap();
public slots:
    void timerUpdateSLOT();
    void trackAddUpdateTimerUpdateSLOT();

private:
    Ui::MainWindow *ui;
    QgsMapCanvas* mapCanvas;
    QVBoxLayout *playout;
    //QgsRasterLayer *lRaster;
     QgsVectorLayer *lVectorLayer;
    QgsVectorLayer *lMemoryBased;
    QTimer *timer;
    QTimer *trackAddUpdateTimer;
    long timerCounter=0;
    int featureIdCnt = 4;

};
#endif // MAINWINDOW_H
