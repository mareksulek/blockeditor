#include "item/layout.h"
#include "style/style.h"
#include <QDebug>

#include "scene/blockscene.h"



Layout::Layout(QString type, Style *style, QGraphicsLayoutItem *parent)
    : QObject(),
      QGraphicsLinearLayout( parent),
      AbstractElement(type, style, dynamic_cast<QGraphicsLinearLayout*>(parent)),
      QGraphicsItem(dynamic_cast<QGraphicsItem*>(parent))
{
    //qDebug() << "---------   "<< type << " " << style;
    if (OrientationEnum::horizontal == _style->getOrientation()) {
        this->setOrientation( Qt::Horizontal);
    } else {
        setSpacing(6);
        this->setOrientation( Qt::Vertical);
    }
    if (type == "block"){
        setContentsMargins(20, 0, 0, 0);
        setAcceptedMouseButtons(0);
    }
    if(type=="root")
        setAcceptedMouseButtons(0);
    //_childLayouts = new  QList<Layout*>();
    setInstantInvalidatePropagation(true);
    setVisible(true);
    if(style->getIsTransparent()){
        setPaintEnable(false);
        setAcceptedMouseButtons(0);
    }else{
        setPaintEnable(true);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
    }
    _activeContext = true;
    //setOpacity(0.5);

}


void Layout::childItemChanged() {
    this->updateGeometry();
}

void Layout::activeContextChanged(Layout *context) {
    if(context->isAncestorOf(this) || context==this){
        _activeContext = true;
    }else{
        _activeContext = false;
    }
}

QSizeF Layout::elementSizeHint(Qt::SizeHint which) const
{
    return sizeHint(which);
}


// **************************************************
// redefine QGRaphics rectangular method

QSizeF Layout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return  QGraphicsLinearLayout::sizeHint(which);
}

void Layout::setGeometry(const QRectF &geom)
{
    prepareGeometryChange();
    QGraphicsLinearLayout::setGeometry( geom);
    //setPos(0,0);
}

QRectF Layout::boundingRect() const
{
    return QRectF(geometry().adjusted(0,0,5,22));
}

void Layout::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    if (isPaintEnabled()) {
        //qDebug() << "paint " << toString();


        /*QRectF frame(geometry());
        frame.translate(3,20);
        frame.adjust(-5,-5,1,1);*/
        QBrush background = QBrush(_style->getBackground(), Qt::SolidPattern);
        if(isSelected()){
            background.setStyle(Qt::Dense4Pattern);
        }
        if(!isActiveContext() && background.color().alphaF()>0){
            background.setColor(QColor(240,240,240));
        }
        if(_style->getOrientation() == OrientationEnum::vertical){
            QRectF frame(childrenBoundingRect());
            frame.translate(3,20);
            frame.adjust(-5,-5,1,1);
            qreal header_height = firstLastItem(true)->minimumHeight();
            QPainterPath ppath;
            //ppath.addRect(frame.left(), frame.top(), frame.width(), header_height+6);
            ppath.addRect(frame.left(), frame.top(), 20, frame.height()-2);
            ppath.addRect(frame.left(), frame.bottom()-header_height-8, frame.width(), header_height+6);
            for(QGraphicsItem *item: childItems()){
                if(Layout *l = dynamic_cast<Layout *>(item)){
                    if(l->getType()=="header"){
                        ppath.addRect(frame.left(), l->boundingRect().top()+15, frame.width(), header_height+6);
                    }
                }
            }
            ppath.setFillRule(Qt::WindingFill);
            painter->fillPath(ppath, background);
            if(_style->getBackground().alpha()){
                painter->drawPath(ppath.simplified());
            }
        }else{
            QRectF frame(geometry());
            frame.translate(3,20);
            frame.adjust(-5,-5,1,1);
            painter->fillRect(frame, background);
            if(_style->getBackground().alpha()){
                painter->drawRect(frame);
            }
        }
        //painter->drawRoundedRect(frame, 3.0, 3.0);
    }
}

void Layout::updateChildNeighbors()
{
    qDebug() << "  chids: " << count();
    AbstractElement *previous = NULL;
    for (int i=0; i < count(); i++) {
        if (AbstractElement *myItem = dynamic_cast <AbstractElement*>(itemAt(i))) {
            //if(NULL == previous) { qDebug() << "NULL           " << myItem;}
            //else { qDebug() << previous << "    " << myItem;}

            if( NULL != previous ) {
                previous->setNext(myItem);
            }
            myItem->setPrevius(previous);
            previous = myItem;

            // recursively update children
            if (Layout *layout = dynamic_cast <Layout*>(itemAt(i))) {
                layout->updateChildNeighbors();
            }
        }
    }

    qDebug() << " Neighbors controll:";

    qDebug() << "  chids: " << count();
    for (int i=0; i < count(); i++) {
        if (AbstractElement *myItem = dynamic_cast <AbstractElement*>(itemAt(i))) {
            if (myItem->getPrevius() != NULL) {
                qDebug() << "previous " << myItem->getPrevius() << "   " << myItem->getPrevius()->getType() << "  " << myItem->getPrevius()->isLayoutE();}
            if (myItem->getNext() != NULL) {
                qDebug() << "next     " << myItem->getNext() << "   " << myItem->getNext()->getType() << "  " << myItem->getNext()->isLayoutE();}

        }
    }
    //qDebug() << "THIS  " << this << "   " << this->getType();
    if( this->getLayoutParrent() != NULL){
        //qDebug() << "THIS  " << this->getLayoutParrent();
    }
    qDebug() << "";
}

bool Layout::isLayoutE() const
{
    return true;
}

QGraphicsLayoutItem  *Layout::firstLastItem(bool first) const
{
    if (first) {
        return itemAt(0);
    } else {
        return itemAt(count()-1);
    }
}

int Layout::textLength(bool length) const
{
    if(length) {
        int sum = 0;
        for ( int i=0; i < count(); i++) {
            if (AbstractElement *element = dynamic_cast <AbstractElement*>(itemAt(i))) {
                sum += element->textLength(length);
            }
        }
        return sum;
    } else {
        return 0;
    }
}

QString Layout::textE() const
{
    QString text;
    for ( int i=0; i < count(); i++) {
        if (AbstractElement *element = dynamic_cast <AbstractElement*>(itemAt(i))) {
            text += element->textE();
        }
    }
    return text;
}

int Layout::indexOf(AbstractElement *element)
{
    for ( int i=0; i < count(); i++) {
        if (dynamic_cast <AbstractElement*>(itemAt(i)) == element) {
            return i;
        }
    }
    return -1;
}

void Layout::setPaintEnable( bool enablePaint ) {
    _enablePaint = enablePaint;
    update();
}

QPixmap Layout::toPixmap() {
    BlockScene *scene = BlockScene::instance();
    QRectF r = boundingRect();
    r.adjust(0,0,5,0);

    QPixmap pixmap(r.width(), r.height());
    pixmap.fill(QColor(0, 0, 0, 10));
    QPainter painter(&pixmap);

    scene->render(&painter, QRectF(), sceneBoundingRect().translated(-3,5));
    painter.end();

    return pixmap.copy(0,10,r.width(),r.height()-13);
}

bool Layout::isActiveContext() const {
    return _activeContext;
}

/*
QSizeF Layout::childrenSizeHint(Qt::SizeHint which) const
{
    //qDebug() << "childrenSizeHint";
    qreal h = 0, w = 0;
    foreach (AbstractElement* child, _childLayouts) {
        QSizeF size = child->elementSizeHint(which);
        if( Qt::Vertical == orientation()) {
            h += size.height();
            w = w < size.width() ? size.width() : w;
        } else {
            w += size.width();
            h = h < size.height() ? size.height() : h;
        }

    }
    //qDebug() <<"CHSH  "<< w << "  " << h;
    return QSizeF(w,h);
}

void Layout::addLayoutChild(AbstractElement *child)
{
    _childLayouts.append(child);
}

QList<AbstractElement *> Layout::getChildLayouts() const
{
    return _childLayouts;
}
*/
