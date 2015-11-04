#ifndef LAYOUT_H
#define LAYOUT_H

#include "abstractelement.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsItem>
#include <QObject>

class Style;

class Layout :
        public QObject,
        public QGraphicsLinearLayout,
        public AbstractElement,
        public QGraphicsItem
{
    Q_OBJECT

public:
    explicit Layout(QGraphicsLayoutItem *parent = 0);

    // inherited from QGraphicsLinearLayout
    QSizeF  sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    void    setGeometry(const QRectF &geom);

    // inherited from QGraphicsItem
    QRectF  boundingRect() const Q_DECL_OVERRIDE;
    void    paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/) Q_DECL_OVERRIDE;

    // reimplemented from Abstract Element
    QSizeF  elementSizeHint(Qt::SizeHint which) const;
    bool    isLayoutE() const;
    int     textLength(bool length) const;
    QString textE() const;
    QPixmap toPixmap();
    void    setStyleE(Style *style);
    void    setPaintEnable( bool enablePaint );

    // class methods
    void    updateChildNeighbors();
    int     elementCount();
    int     indexOf(AbstractElement *element);
    void    insertOnStart( AbstractElement *newElement);
    void    insertBehind( AbstractElement *oldElement, AbstractElement *newElement);
    void    insertBefore( AbstractElement *oldElement, AbstractElement *newElement);
    void    removeElement( AbstractElement *element);
    QGraphicsLayoutItem  *firstLastChildrenElement(bool first) const;

public slots:
    void childItemChanged();

protected:

    QString _layoutType;

};

#endif
