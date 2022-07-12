#define _USE_MATH_DEFINES
#include <cmath>
#include "compassroseitem.h"
#include "qgspoint.h"
#include "qpainter.h"
#include "qpainterpath.h"

CompassRoseItem::CompassRoseItem(QgsMapCanvas *mapCanvas):QgsMapCanvasItem(mapCanvas)
{
    this->center = QgsPoint(0,0);
    this->size = 100;

}

void CompassRoseItem::setCenter(QgsPoint pCenter)
{
    this->center = pCenter;
}

void CompassRoseItem::setSize(int pSize)
{
    this->size = pSize;
}

void CompassRoseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int fontSize = 18 * size/100;

    painter->setFont(QFont("Times",fontSize,75));
    QFontMetrics metrics = painter->fontMetrics();
    int labelSize = metrics.height();
    int margin = 5;

    double x = center.x();
    double y = center.y();
    int s = size - labelSize - margin;
    QPainterPath path;
    path.moveTo(x,y-s*0.23);
    path.lineTo(x - s*0.45, y-s*0.45);
    path.lineTo(x-s*0.23,y);
    path.lineTo(x - s * 0.45, y + s * 0.45);
    path.lineTo(x, y + s * 0.23);
    path.lineTo(x + s * 0.45, y + s * 0.45);
    path.lineTo(x + s * 0.23, y);
    path.lineTo(x + s * 0.45, y - s * 0.45);
    path.closeSubpath();

    painter->fillPath(path, QColor("light gray"));

    path = QPainterPath();
    path.moveTo(x, y - s);
    path.lineTo(x - s * 0.18, y - s * 0.18);
    path.lineTo(x - s, y);
    path.lineTo(x - s * 0.18, y + s * 0.18);
    path.lineTo(x, y + s);
    path.lineTo(x + s * 0.18, y + s * 0.18);
    path.lineTo(x + s, y);
    path.lineTo(x + s * 0.18, y - s * 0.18);
    path.closeSubpath();

    painter->fillPath(path, QColor("black"));

    int labelX = x - metrics.width("N")/2;
    int labelY = y - size + labelSize - metrics.descent();
    painter->drawText(QPoint(labelX, labelY), "N");

    labelX = x - metrics.width("S")/2;
    labelY = y + size - labelSize + metrics.ascent();
    painter->drawText(QPoint(labelX, labelY), "S");

    labelX = x - size + labelSize/2 - metrics.width("E")/2;
    labelY = y - metrics.height()/2 + metrics.ascent();
    painter->drawText(QPoint(labelX, labelY), "E");

    labelX = x + size - labelSize/2 - metrics.width("W")/2;
    labelY = y - metrics.height()/2 + metrics.ascent();
    painter->drawText(QPoint(labelX, labelY), "W");

}

QRectF CompassRoseItem::boundingRect() const
{
    return QRectF(center.x() - size/2,
                  center.y() - size/2,
                  center.x() + size/2,
                  center.y() + size/2);
}

void CompassRoseItem::paint(QPainter *painter)
{

}
