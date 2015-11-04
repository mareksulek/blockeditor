#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsTextItem>
#include <QGraphicsLayoutItem>
#include <QSizeF>
#include <QObject>

#include "item/abstractelement.h"


class Item :   public QGraphicsLayoutItem, public QGraphicsTextItem, public AbstractElement
{

public:
    explicit Item(Layout *parent = 0, QString text = "");

    // reimplement from QGraphicsLayoutItem
    QSizeF  sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    void    setGeometry(const QRectF &geom);


    // reimplemented from Abstract Element
    QSizeF  elementSizeHint(Qt::SizeHint which) const;
    bool    isLayoutE() const;
    int     textLength( bool length = true) const;
    QString textE() const;
    QPixmap toPixmap();
    void    setStyleE(Style *style);


    // methods
    QString textOnLineForPos(int pos, bool toRight) const;
    int cursorPositionIn(AbstractElement *topParent) const;

    // reimplement from QGraphicsTextItem
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

public slots:
    void textChanged( int pos, int charsRemoved, int charsAdded);

};
#endif
