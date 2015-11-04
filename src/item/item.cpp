#include "item/item.h"
#include <QDebug>

#include <QGraphicsItem>
#include <QFontDatabase>
#include <QStyleOptionGraphicsItem>
#include <item/layout.h>
#include "style/style.h"
#include "scene/blockscene.h"
#include "scene/command/writeitemcommand.h"


Item::Item(Layout *parent, QString text)
    : QGraphicsLayoutItem(parent),
      QGraphicsTextItem(dynamic_cast<QGraphicsItem*>(parent)),
      AbstractElement(parent)
{
    document()->setPlainText(text);
    setTextInteractionFlags( Qt::TextEditorInteraction);

    //setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);


    // draging
    setAcceptDrops(true);

    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(fixedFont);

    connect( document(), &QTextDocument::contentsChange, this , &Item::textChanged );

    // setGeometry();
}


// ----------------------------------------------------------
// -- reimplement from QGraphicsLayoutItem ------------------
// ----------------------------------------------------------

QSizeF Item::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {
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

void Item::setGeometry(const QRectF &geom) {
    //qDebug() << "GG" << geom.size().width() <<"  "<< geom.size().height() << _type ;
    //qDebug() << "  " << geom.topLeft().x() <<"  "<< geom.topLeft().y();
    //qDebug() << "  " << geom.topLeft().x() <<"  "<< geom.topLeft().y()+boundingRect().size().width();
    //prepareGeometryChange();
    //QGraphicsLayoutItem::setGeometry(geom);
    setPos(geom.bottomLeft());

}

// ----------------------------------------------------------
// -- reimplement from QGraphicsTextItem --------------------
// ----------------------------------------------------------

void Item::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {

    QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(option);
    o->state &= ~QStyle::State_HasFocus;
    o->state &= ~QStyle::State_Selected;

    QGraphicsTextItem::paint(painter, o, widget);
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsTextItem::mousePressEvent(event);
}

void Item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    setCursor(Qt::ArrowCursor);
    QGraphicsTextItem::mouseReleaseEvent(event);
}


// ----------------------------------------------------------
// -- reimplemented from Abstract Element -------------------
// ----------------------------------------------------------

QSizeF Item::elementSizeHint(Qt::SizeHint which) const {
    return sizeHint(which);
}

bool Item::isLayoutE() const {
    return false;
}


int Item::textLength(bool length) const {
    if(length) {
        return toPlainText().length();
    } else {
        return 0;
    }
}

QString Item::textE() const {
    return toPlainText();
}


void Item::setStyleE(Style *style) {
    AbstractElement::setStyleE(style);

    if (style->getIsColor()){
        setDefaultTextColor(Qt::blue);
    } else {
        setDefaultTextColor(Qt::black);
    }
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



// ----------------------------------------------------------
// -- class methods -----------------------------------------
// ----------------------------------------------------------

void Item::textChanged(int pos, int charsRemoved, int charsAdded) {
    if (!signalsBlocked()) {
        qDebug() <<"text changed, but signal blocked="<< signalsBlocked();
        if (charsRemoved!=charsAdded){
            BlockScene::instance()->addCommand(new WriteItemCommand(this, pos, charsRemoved, charsAdded));
        }
    }

    if (getLayoutParrent()) {
        updateGeometry();
        getLayoutParrent()->invalidate();
    }
}

QString Item::textOnLineForPos(int pos, bool toRight) const {
    QString text;
    if (toRight) {
        text = toPlainText().mid(pos);
    } else {
        text = toPlainText().mid(0,pos);
    }

    Layout *parent = getLayoutParrent();
    const AbstractElement *element = this;

    while (parent != NULL && Qt::Horizontal == parent->orientation()) {
        AbstractElement *neighbor = element->nextPrevius(toRight);
        while (neighbor) {
            if (toRight) {
                text += neighbor->textE();
            } else {
                text = neighbor->textE() + text;
            }

            neighbor = neighbor->nextPrevius(toRight);
        }
        element = parent;
        parent = parent->getLayoutParrent();
    }

    if (parent == NULL) {
        qDebug() << "WARNING!!! editing - add Enter, build text, parent=NULL";
    }

    return text;
}

int Item::cursorPositionIn(AbstractElement *topParent) const {
    int pos = textCursor().position();
    const AbstractElement *element = this;

    while (element != NULL && element != topParent) {
        AbstractElement *neighbor = element->getPrevius();
        while (neighbor) {
            pos += neighbor->textLength();
            neighbor = neighbor->getPrevius();
        }
        element = element->getLayoutParrent();
    }
    return pos;
}

