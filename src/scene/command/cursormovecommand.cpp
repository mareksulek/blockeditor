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

    switch (_event->key()){
    case Qt::Key_Left:
        horCursorMovement( false);
        break;
    case Qt::Key_Right:
        horCursorMovement( true);
        break;
    case Qt::Key_Up:
        verCursorMovement( false);
        break;
    case Qt::Key_Down:
        verCursorMovement( true);
        break;
    }
}


void CursorMoveCommand::horCursorMovement(bool toNext)
{
    QTextCursor cursor = _item->textCursor();
    AbstractElement *targed;
    if (_item->textLength(toNext) == cursor.position()) {
        targed = _item->nextPreviousAlsoVert(toNext);
        if (NULL == targed) {
            return;
        }

        Item *ite = dynamic_cast <Item*>(targed);
        ite->setFocus();
        cursor = ite->textCursor();
        cursor.setPosition(ite->textLength(!toNext));
        ite->setTextCursor(cursor);

    } else {
        if(toNext){
            cursor.movePosition(QTextCursor::NextCharacter);//, QTextCursor::KeepAnchor);
        } else {
            cursor.movePosition(QTextCursor::PreviousCharacter);//, QTextCursor::KeepAnchor);
        }
        _item->setTextCursor(cursor);
    }
}

void CursorMoveCommand::verCursorMovement(bool down) {

    int linePos = _item->textCursor().position();
    Layout *parent = _item->getLayoutParrent();
    const AbstractElement *element = _item;

    // find proper parent and calculate position on line
    while (parent
           && ( Qt::Horizontal == parent->orientation()
                || (Qt::Vertical == parent->orientation() && element->nextPrevius(down) == NULL))) {

        if (Qt::Vertical != parent->orientation()) {
            AbstractElement *neighbor = element->getPrevius();
            while (neighbor) {
                linePos += neighbor->textLength();
                neighbor = neighbor->getPrevius();
            }
        }
        element = parent;
        parent = parent->getLayoutParrent();
    }


    if (!parent) {
        // top / end
        return;
    }

    // up / down movement
    AbstractElement *targed = element->nextPrevius(down);

    // find proper position on new line
    AbstractElement *child = NULL;

    while (targed->isLayoutE()) {
        Layout *layout = dynamic_cast <Layout*>(targed);
        if ( Qt::Vertical == layout->orientation()) {
            if (down) {
                targed = dynamic_cast<AbstractElement *>(layout->itemAt(0));
            } else {
                targed = dynamic_cast<AbstractElement *>(layout->itemAt(layout->count()-1));
            }

        } else {
            child = dynamic_cast<AbstractElement *>(layout->itemAt(0));
            int len = skipChild( child, linePos);
            while ( 0 < len) {
                linePos -= len;
                child = child->getNext();
                len = skipChild(child, linePos);
            }
            targed = child;
        }
    }

    int len = targed->textLength();
    linePos = linePos < len ? linePos : len;

    Item *ite = dynamic_cast <Item*>(targed);
    ite->setFocus();
    QTextCursor cursor = ite->textCursor();
    cursor.setPosition(linePos);
    ite->setTextCursor(cursor);
}


int CursorMoveCommand::skipChild(AbstractElement *child, int linePos) {
    if (child == NULL) {
        return 0;
    }
    // layout
    if (child->isLayoutE()) {
        Layout *layout = dynamic_cast <Layout*>(child);
        if ( Qt::Horizontal == layout->orientation()) {
            int len = child->textLength();
            if (len < linePos
                    && child->getNext()) {
                return len;
            }
        }
        return 0;

        // item
    } else {
        int len = child->textLength();
        if (len < linePos
                && child->getNext()) {
            return len;
        }
    }
    return 0;
}

void CursorMoveCommand::undo() {
    qDebug() << "UNDO cursorMoveCommand ";
}
