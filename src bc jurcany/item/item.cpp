#include "item/item.h"
#include <QDebug>
#include <QFontMetrics>
#include <QTextDocument>
#include <QTextCursor>
#include <QMimeData>
#include <QDrag>
#include <QFontDatabase>

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneWheelEvent>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>
#include <QDrag>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <item/layout.h>
#include "style/style.h"
#include "scene/blockscene.h"


#include "scene/mimedata.h"



Item::Item(QString type, QString text, Style *style, QGraphicsLinearLayout *parent)
    :  QGraphicsLayoutItem(parent), QGraphicsTextItem(), AbstractElement(type, style, parent)
{
    //_text = text.trimmed();
    _text = text;


    _document = this->document();
    _document->setPlainText(_text);
    setTextInteractionFlags( Qt::TextEditorInteraction);

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIsSelectable);

    if (style->getIsColor()){
        setDefaultTextColor(Qt::blue);
    }
    // draging
    setAcceptDrops(true);
    //color = QColor(Qt::lightGray);



    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(fixedFont);

QConnect:connect( _document, &QTextDocument::contentsChanged, this , &Item::textUpdatedSlot );

    // setGeometry();
}

void Item::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

    QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(option);
    o->state &= ~QStyle::State_HasFocus;
    o->state &= ~QStyle::State_Selected;

    QGraphicsTextItem::paint(painter, o, widget);
}

void Item::setGeometry(const QRectF &geom) {

    //qDebug() << "GG" << geom.size().width() <<"  "<< geom.size().height() << _type ;
    //qDebug() << "  " << geom.topLeft().x() <<"  "<< geom.topLeft().y();
    //qDebug() << "  " << geom.topLeft().x() <<"  "<< geom.topLeft().y()+boundingRect().size().width();
    //prepareGeometryChange();
    //QGraphicsLayoutItem::setGeometry(geom);
    setPos(geom.bottomLeft());
}

void Item::textUpdatedSlot() {
    updateGeometry();
    getLayoutParrent()->invalidate();
}

QSizeF Item::elementSizeHint(Qt::SizeHint which) const
{
    return sizeHint(which);
}


QSizeF Item::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QFontMetrics fm(font());
    QString text = toPlainText();
    //text.
    //qDebug() << "basicBlock.sizeHint" << boundingRect().size().width() << "  " << boundingRect().size().height() << _type;
    //qDebug() << "basicBlock.sizeHint" << fm.width(toPlainText()) << "  " << fm.height();

    switch (which) {
    case Qt::MinimumSize:
    case Qt::PreferredSize:
    case Qt::MaximumSize:
        //return this->boundingRect().size();
        return QSize(fm.width(text), fm.height());
    }
    return this->boundingRect().size();
    //return constraint;
}

bool Item::isLayoutE() const
{
    return false;
}


int Item::textLength(bool length) const
{
    if(length) {
        return toPlainText().length();
    } else {
        return 0;
    }
}

QString Item::textE() const
{
    return toPlainText();
}




// ---------------
void Item::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsTextItem::mousePressEvent(event);
}

void Item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void Item::focusInEvent(QFocusEvent *event) {
    QGraphicsTextItem::focusInEvent(event);
    BlockScene *bs = dynamic_cast<BlockScene *>(scene());
    Layout *l = dynamic_cast<Layout *>(parentLayoutItem());
    while(l = dynamic_cast<Layout *>(l->parentLayoutItem())){
        if(l->styleE()->getIsContext()){
            qDebug() << "DEBUG_CONTEXT" << l->textE();
            bs->setContext(l);
            break;
        }
    }
}

void Item::focusOutEvent(QFocusEvent *event) {
    BlockScene *bs = dynamic_cast<BlockScene *>(scene());
    bs->unsetContext();
    QGraphicsTextItem::focusOutEvent(event);
}


QPixmap Item::toPixmap() {
    QSizeF elementSize = elementSizeHint(Qt::MinimumSize);
    QPixmap pixmap(elementSize.width()+3, elementSize.height()+3);
    pixmap.fill(QColor(0,0,0,10));

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(-2, -3);

    QStyleOptionGraphicsItem opt;
    QWidget wid;
    paint(&painter, &opt, &wid);

    QRectF rect(3,3,elementSize.width(),elementSize.height());
    painter.drawRoundedRect(rect, 3.0, 3.0);

    painter.end();
    return pixmap;
}
