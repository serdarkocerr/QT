#include "pantool.h"


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
