#ifndef MINIMAPVIEW_H
#define MINIMAPVIEW_H

#include <QGraphicsView>

class BlockScene;

class MiniMapView : public QGraphicsView
{
    Q_OBJECT

public:
    MiniMapView();
    ~MiniMapView();

public slots:
    void updateScene(const QList<QRectF> & rects);
};

#endif // MINIMAPVIEW_H
