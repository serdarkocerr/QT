#ifndef COMPASSROSEITEM_H
#define COMPASSROSEITEM_H
#define _USE_MATH_DEFINES
#include <cmath>
#include "qgsmapcanvasitem.h"
#include "qgspoint.h"

class CompassRoseItem : public QgsMapCanvasItem
{
public:
    CompassRoseItem(QgsMapCanvas *mapCanvas);

    void setCenter(QgsPoint pCenter);
    void setSize(int pSize);





    QgsPoint center;
    int size;


    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;

    // QgsMapCanvasItem interface
protected:
    void paint(QPainter *painter) override;
};

#endif // COMPASSROSEITEM_H
