#ifndef SCENEEVENTOBSERVER_H
#define SCENEEVENTOBSERVER_H

#include <QGraphicsItem>

class QKeyEvent;
class Command;

class SceneEventObserver : public QGraphicsItem{
public:
    explicit SceneEventObserver(QGraphicsItem *parent = NULL);

protected:
    QRectF boundingRect() const { return QRectF(); }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) {}

    bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event );

private:
    Command *keyCommand(QGraphicsItem *watched, QKeyEvent *event);

};

#endif
