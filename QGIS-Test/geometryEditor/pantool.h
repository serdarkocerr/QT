#ifndef PANTOOL_H
#define PANTOOL_H

#define _USE_MATH_DEFINES
#include <cmath>

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

class PanTool : public QgsMapTool
{
public:
    PanTool(QgsMapCanvas *canvas);

    void canvasMoveEvent(QgsMapMouseEvent *e);
    void canvasReleaseEvent(QgsMapMouseEvent *e);

};

#endif // PANTOOL_H
