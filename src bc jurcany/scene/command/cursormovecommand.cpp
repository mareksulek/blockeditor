#include "scene/command/cursormovecommand.h"


#include <QDebug>
#include <QKeyEvent>
#include <QTextCursor>
#include "QGraphicsLayoutItem"

#include "item/item.h"
#include "item/layout.h"
#include "item/abstractelement.h"
#include "style/style.h"


CursorMoveCommand::CursorMoveCommand(QGraphicsItem *watched, QKeyEvent *event)
    : Command()
{
    _item = static_cast<Item *>(watched) ;
    _event = event;

}


void CursorMoveCommand::execute() {
    qDebug() << "EXE cursorMoveCommand";

    QTextCursor cursor = _item->textCursor();

    switch (_event->key()){
    case Qt::Key_Left:
        horCursorMovement(cursor, false);
        break;
    case Qt::Key_Right:
        horCursorMovement(cursor, true);
        break;
    case Qt::Key_Up:
        verCursorMovement(cursor, false);
        break;
    case Qt::Key_Down:
        verCursorMovement(cursor, true);
        break;
    }
}


void CursorMoveCommand::horCursorMovement(QTextCursor &cursor, bool toNext)
{
    AbstractElement *targed;
    if (_item->textLength(toNext) == cursor.position()) {
        targed = _item->nextPrevius(toNext);
        if (NULL == targed) {
            targed = _item->getLayoutParrent();
            while (NULL != targed && NULL == targed->nextPrevius(toNext)) {
                targed = targed->getLayoutParrent();
            }
            if (NULL == targed) {
                return;
            }
            targed = targed->nextPrevius(toNext);
        }
        while (targed->isLayoutE()) {
            Layout *lay = dynamic_cast <Layout*>(targed);
            QGraphicsLayoutItem  *layout = lay->firstLastItem(toNext);
            targed = dynamic_cast <AbstractElement*>(layout);

        }
        Item *ite = dynamic_cast <Item*>(targed);
        ite->setFocus();
        cursor = ite->textCursor();
        cursor.setPosition(ite->textLength(!toNext));
        ite->setTextCursor(cursor);

    } else {
        if(toNext){
            cursor.movePosition(QTextCursor::NextCharacter);
        } else {
            cursor.movePosition(QTextCursor::PreviousCharacter);
        }
        _item->setTextCursor(cursor);
    }
}

void CursorMoveCommand::verCursorMovement(QTextCursor &cursor, bool down) {
    AbstractElement *targed = static_cast<AbstractElement *>(_item) ;;
    int linePos = cursor.position();
    // qDebug() << "";
    //qDebug() << "start " << linePos << "  " << getType() << "  " << toPlainText();
    // left
    while( NULL != targed) {
        AbstractElement *parrent = targed->getLayoutParrent();
        if( NULL != parrent && OrientationEnum::vertical == parrent->styleE()->getOrientation()) {
            break;
        }
        while ( NULL != targed->nextPrevius(false)) {
            targed = targed->nextPrevius(false);
            linePos += targed->textLength();
            //qDebug() << " + "<< targed->textLength() << " = " << linePos << "    " << targed->getType();
        }

        //qDebug() << "up parrent "<< targed->getType() << "  " << targed->textE();
        targed = parrent;

    }
    // down/up


    if (NULL!= targed) {qDebug() << "1. up/down"   << linePos << "    " << targed << "  " << targed->getType() << "  " << targed->textE();}
    while (NULL != targed && NULL == targed->nextPrevius(down)) {  // there can be more vertical
        targed = targed->getLayoutParrent();
        //if (NULL!= targed) {qDebug() << "2. up/down"   << linePos << "    " << targed << "  " << targed->getType() << "  " << targed->textE();}
    }
    if (NULL == targed) { return; }
    targed = targed->nextPrevius(down);


    //qDebug() << "3. up/down"   << linePos << "    " << targed << "  " << targed->getType() << "  " << targed->textE();
    // right
    int tlen;
    while (targed->isLayoutE()) {
        //qDebug() << "down child    " << targed->getType();
        bool isHorizontal = OrientationEnum::horizontal == targed->styleE()->getOrientation();

        Layout *lay = dynamic_cast <Layout*>(targed);
        targed = dynamic_cast<AbstractElement*>(lay->itemAt(0)); // TODO no children?

        tlen = targed->textLength();
        while (isHorizontal
               && tlen < linePos
               && NULL != targed->nextPrevius(true)) {
            qDebug() << " - "<< tlen << " = " << linePos << "    " << targed->getType();
            targed = targed->nextPrevius(true);
            linePos -= (tlen);
            tlen = targed->textLength();
        }
    }


    Item *ite = dynamic_cast <Item*>(targed);
    qDebug() << "final:" << targed->getType() << "  " << ite->toPlainText();
    ite->setFocus();
    cursor = ite->textCursor();
    tlen = targed->textLength();
    linePos = linePos < tlen ? linePos : tlen;
    cursor.setPosition(linePos);
    ite->setTextCursor(cursor);
}
