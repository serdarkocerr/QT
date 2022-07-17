#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <QMainWindow>
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
#include <pantool.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow;
class ICallBack{
   public :
    virtual void onGeometryAdded() = 0;
    virtual void onGeometryChanged() = 0;
    virtual void onGeometryDeleted() = 0;

};

/**
    we implemented EditTool, which lets the user click and drag to move the existing feature's vertices.
    Our editing tool lets the user add new vertices by double-clicking on a line segment,
    and delete vertices by right-clicking on the same line segment.



**/
class EditTool : public QgsMapTool {
public:
    QgsMapCanvas *canvas;


      EditTool(QgsMapCanvas *canvas, QgsVectorLayer *layer, ICallBack *callback);

      struct CapturedPoints{
          QgsPointXY toMapCoordinatePoint;
          QgsPointXY toLayerCoordinatePoint;
      };



private:

       QgsVectorLayer   *pLayer;
       ICallBack *callbackFunc;
       bool dragging = false;
       QgsFeature feature = NULL;
       int vertex  = -1000;





       // QgsMapTool interface
public:
       void canvasMoveEvent(QgsMapMouseEvent *e) override;
       void canvasDoubleClickEvent(QgsMapMouseEvent *e) override;
       void canvasPressEvent(QgsMapMouseEvent *e) override;
       void canvasReleaseEvent(QgsMapMouseEvent *e) override;
       CapturedPoints transformCoordinates(QPoint canvasPoint);
       double calculateTolerance(QPoint pos);
       QgsFeature findFeatureAt(QPoint pos);
       void moveVertexTo(QPoint pos);
       void deleteVertex(QgsFeature feature, int vertex);



};


/**
 A QgsRubberBand is a map canvas item that draws a geometry on top of the map.
 Since a rubber band draws its entire geometry in a single color and style,
 we have to use two rubber bands in our capture tool:
 one that draws the already captured part of the geometry
 and a second temporary rubber band that extends the geometry out to the current mouse position.

 CaptureTool:
    - It will have a capture mode that indicates whether the user is creating a LineString or a Polygon geometry.
    - The user can press the Backspace or Delete key to remove the last vertex added.
    - The user can press the Enter or Return key to finish the capturing process.
    - If we are capturing a Polygon, the geometry will be closed when the user finishes capturing.
      This means that we add an extra point to the geometry so that the outline begins and ends at the same point.
    - When the user finishes capturing a geometry, the geometry will be added to the layer,
      and a callback function will be used to tell the application that a new geometry has been added.

 canvasReleaseEvent():
      which responds to a left-click by adding a new vertex to the geometry,
      and to a right-click by finishing off the capture process and then adding the geometry to the map layer.
      We implement this behavior in the canvasReleaseEvent() method, rather than canvasPressEvent(),
      because we want the vertex to be added when the user releases the mouse button, rather than when they initially press it.
 transformCoordinates():
      method, which converts from a mouse position, which is in canvas coordinates, to map and layer coordinates
      If, for example, the mouse is currently at position (17,53) on the canvas, this may translate to a map and layer coordinate of lat=37.234 and long=-112.472.
      As the map and layer might use different coordinate reference systems, we calculate and return the coordinates for both


**/
class CaptureTool : public QgsMapTool {
public:
    enum CaptureMode {
        CAPTURE_LINE = 0,
        CAPTURE_POLYGON
    };
    struct CapturedPoints{
        QgsPointXY toMapCoordinatePoint;
        QgsPointXY toLayerCoordinatePoint;
    };


        QgsMapCanvas *canvas;

       CaptureTool(QgsMapCanvas *canvas, QgsVectorLayer *layer, ICallBack *callback, CaptureMode  captureMode);


private:

       QgsVectorLayer   *pLayer;
       ICallBack *callbackFunc;
       CaptureMode pCaptureMode;
       QgsRubberBand *rubberBand;
       QgsRubberBand *tempRubberBand;
       QList<CapturedPoints> capturedPoints;
       bool capturing = false;




       // QgsMapTool interface
public:
       void canvasReleaseEvent(QgsMapMouseEvent *e) override;
       void canvasMoveEvent(QgsMapMouseEvent *e) override;
       void keyPressEvent(QKeyEvent *e) override;


       void startCapturing();
       void stopCapturing();
       void addVertex(QPoint canvasPoint);
       QList<CapturedPoints> getCapturedGeometry();
       void geometryCaptured(QList<CaptureTool::CapturedPoints> points);

       CapturedPoints transformCoordinates(QPoint canvasPoint);
       void removeLastVertex();



};

/**
 the code required to delete a Point feature will also work for other types of geometries,
 so we don't need to implement separate DeletePointTool, DeleteLineTool, and DeletePolygonTool classes
**/
class DeleteTool : public QgsMapToolIdentify {
public:
    DeleteTool(QgsMapCanvas *canvas, QgsVectorLayer *layer, ICallBack *callback);

    QgsMapCanvas *canvas;

private :
    QgsVectorLayer   *pLayer;
    ICallBack *callbackFunc;
    QgsFeature feature = NULL;


    // QgsMapTool interface
public:
    void canvasPressEvent(QgsMapMouseEvent *e) override;
    void canvasReleaseEvent(QgsMapMouseEvent *e) override;
};


/**
 We subclass QgsMapToolIdentify for this map tool.
 This lets us use the identify() method to find the geometry that the user clicked on,
 just like we did in the SelectTool in selectionExplorer project on Github.
**/
class MovePointTool : public QgsMapToolIdentify {
public:
    MovePointTool(QgsMapCanvas *canvas, QgsVectorLayer *layer, ICallBack *callback);

    QgsMapCanvas *canvas;

    // QgsMapTool interface
public:
    void canvasMoveEvent(QgsMapMouseEvent *e) override;
    void canvasPressEvent(QgsMapMouseEvent *e) override;
    void canvasReleaseEvent(QgsMapMouseEvent *e) override;
private:
    QgsVectorLayer   *pLayer;
    ICallBack *callbackFunc;
    bool dragging = false;
    QgsFeature feature = NULL;
    bool moved = false;

};
class AddPointTool : public QgsMapTool {

public:
       AddPointTool(QgsMapCanvas *canvas, QgsVectorLayer *layer/*, void (MainWindow::*callbackFunctPtr)()*/, ICallBack *callback);

        QgsMapCanvas *canvas;


       // QgsMapTool interface
public:
       void canvasReleaseEvent(QgsMapMouseEvent *e) override;
//       void onGeometryAdded();
//       typedef void (MainWindow::*clbk_ptr)(void);
//       clbk_ptr cb_func;
//       void setCallback( clbk_ptr ptr);//func pointer set

private:
    QgsVectorLayer   *pLayer;
    ICallBack *callbackFunc;

};


class MainWindow : public QMainWindow, public ICallBack
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onGeometryAdded() override;
    void onGeometryChanged() override;
    void onGeometryDeleted() override;

    void createAndConnectToolbar();
    void loadMap();
    void setupMapTools();
    void adjustToolbar();

    bool isModified = false;


public slots:
    void quitSLOT();
    void zoomInSLOT();
    void zoomOutSLOT();
    void setPanModeSLOT();

    void editPointLayerSLOT();
    void editLineLayerSLOT();
    void editPolygonLayerSLOT();
    void addFeatureSLOT();
    void editFeatureSLOT();
    void deleteFeatureSLOT();


private:
    Ui::MainWindow *ui;
    QgsMapCanvas* mapCanvas;
    QVBoxLayout *playout;
    QgsVectorLayer *lPolygonVector;
    QgsVectorLayer *lLineVector;
    QgsVectorLayer *lPointVector;
    QgsRasterLayer *lRaster;

    PanTool *panTool;
    AddPointTool *addPointTool;
    MovePointTool *movePointTool;
    DeleteTool *deletePointTool;
    CaptureTool *addLineTool;
    CaptureTool *addPolygonTool;
    DeleteTool *deleteLineTool;
    DeleteTool *deletePolygonTool;
    EditTool *editLineTool;
    EditTool *editPolygonTool;

    QgsVectorLayer *currentEditingLayer;

     QAction *actionEditPoint;
     QAction *actionPan;
     QAction *actionAdd;
     QAction *actionEditLine;
     QAction *actionEditPolygon;
     QAction *actionEdit;
     QAction *actionDelete;
     QAction *actionQuit;

};
#endif // MAINWINDOW_H
