#include "scene/command/writeitemcommand.h"


#include <QDebug>
#include <QKeyEvent>
#include <QTextCursor>
#include <QGraphicsLayoutItem>
#include <QTextDocument>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "item/item.h"
#include "item/layout.h"
#include "item/abstractelement.h"
#include "style/styleutil.h"
#include "scene/blockscene.h"
#include "item/state/elementstate.h"
#include "item/state/elementstable.h"
#include "item/state/elementchanged.h"
#include "scene/parser.h"



WriteItemCommand::WriteItemCommand(QGraphicsItem *watched, int pos, int charsRemoved, int charsAdded)
    : Command()
{
    _item = static_cast<Item *>(watched) ;
    this->pos = pos;
    this->charsAdded = charsAdded;
    this->charsRemoved = charsRemoved;

}


void WriteItemCommand::execute() {

    qDebug() << "\n\n\n\nEXE writeItemCommand";
    qDebug() <<  pos <<"  "<< charsRemoved <<"  "<< charsAdded;

    //    QTextCursor cursor = _item->textCursor();
    //    if (_item->document()->isUndoAvailable()) {
    //        qDebug() << " orig       "<< _item->toPlainText();
    //        qDebug() << " orig-pos   "<<  cursor.position();
    //        _item->document()->undo( &cursor);
    //        qDebug() << " undo       "<< _item->toPlainText();
    //        qDebug() << " undo-pos   "<<  cursor.position();
    //    }

    // ADD
    if ( 0 == charsRemoved || charsAdded > charsRemoved) {
        charsAdded -= charsRemoved;
        charsRemoved = 0;
        qDebug() << "*"<<  pos <<"  "<< charsRemoved <<"  "<< charsAdded;

        if (1 == charsAdded ){
            simpleAddition();
        }
    }

    // REMOVE
    else if (1 == charsRemoved) {
        simpleRemove();
    }
}

void WriteItemCommand::simpleRemove() {
    qDebug() << "  simple remove";
    AbstractElement *parseble = NULL;

    if (_item->textLength() == 0) {
        qDebug() << "remove item";
        AbstractElement *toRemove = _item;
        Layout *parent = _item->getLayoutParrent();

        while (parent != NULL && parent->count() < 2) {
            qDebug() << "up";
            toRemove = parent;
            parent = parent->getLayoutParrent();
        }

        if (parent != NULL) {
            // select else one
            Item *instead = dynamic_cast <Item*>(findInsteadtoSelect());
            instead->setFocus();

            // remove
            parseble = prepareParsableForRemoved( toRemove, _item);
            parent->removeElement(toRemove);

            BlockScene::instance()->removeItem( dynamic_cast<QGraphicsItem *>(toRemove));

            parseble->edited();
        } else {
            parseble = prepareParsable( _item);
            parseble->edited();
        }
    } else {
        parseble = prepareParsable( _item);
        parseble->edited();
    }
}

void WriteItemCommand::simpleAdditionEnter() {
    qDebug() << "ENTER";
    undoSimpleAddition();

    QString leftLine = _item->textOnLineForPos(pos, false);
    QString rightLine = _item->textOnLineForPos(pos, true);
    qDebug() << "left:  "<< leftLine;
    qDebug() << "right: "<< rightLine;


    Layout *vParent = _item->getLayoutParrent();
    AbstractElement *toReplace = _item;
    while (vParent != NULL && Qt::Horizontal == vParent->orientation()) {
        toReplace = vParent;
        vParent = vParent->getLayoutParrent();
    }
    if (vParent == NULL) {
        qDebug() << "WARNING!!! simple addition enter - vertical parent is NULL";
        return;
    }

    bool addLeft = true;
    bool addRight = true;
    if (rightLine.isEmpty()) {
        rightLine = " ";
        addLeft = false;
    }
    if (leftLine.isEmpty()) {
        leftLine = " ";
        addRight = false;
    }

    Item *rightItem = NULL;
    Item *leftItem = NULL;
    if (addRight) {
        qDebug() << "   addd right half";
        rightItem = createItemForInsert( !addLeft, rightLine, vParent);

        vParent->insertBehind(toReplace, rightItem);
        BlockScene::instance()->addItem(rightItem);

        rightItem->setFocus();
        QTextCursor cursor(rightItem->textCursor());
        cursor.setPosition(0);
        rightItem->setTextCursor(cursor);
    }
    if (addLeft) {
        qDebug() << "   addd left half";
        leftItem = createItemForInsert( !addRight, leftLine, vParent);

        vParent->insertBefore(toReplace, leftItem);
        BlockScene::instance()->addItem(leftItem);
    }
    if (addLeft && addRight) {
        qDebug() << "  remove old line";
        vParent->removeElement(toReplace);
        BlockScene::instance()->removeItem( dynamic_cast<QGraphicsItem *>(toReplace));

        AbstractElement *parseble = prepareParsable(vParent, rightItem);
        parseble->edited();
        // TODO ???  rightItem->edited(rightItem);  nieje zaznaceny ako editovany
    }
}

void WriteItemCommand::simpleAddition() {

    qDebug() << "  simple addition";
    QTextCursor cursor(_item->textCursor());
    QChar newChar = _item->document()->characterAt(pos);

    if (newChar.unicode() == 8233) {
        simpleAdditionEnter();
        return; // TODO
    }

    if ( _item->state()->isSpaced() == newChar.isSpace()) {
        AbstractElement *parseble = prepareParsable( _item);
        parseble->edited();

    } else {
        qDebug() << "  different item types, added:" << newChar;
        if (0 == pos) {
            simpleAdditionStartEnd(newChar, true);
        } else if (cursor.atEnd()) {
            simpleAdditionStartEnd(newChar, false);
        } else {
            simpleAdditionMiddle(newChar);
        }
    }
}

void WriteItemCommand::simpleAdditionMiddle(QChar newChar) {
    qDebug() << " add in the middle";
    Layout *parent = _item->getLayoutParrent();

    QString text = _item->toPlainText();
    _item->blockSignals(true);
    _item->setPlainText(text.mid(0,pos));
    _item->blockSignals(false);

    Item *second = NULL;
    Item *third = NULL;
    if (parent->orientation() == Qt::Horizontal) {
        second = createItemForInsert( newChar);
        third = createItemForInsert( !newChar.isSpace(), text.mid(pos+1));

        parent->insertBehind(_item, second);
        parent->insertBehind(second, third);
        BlockScene::instance()->addItem(second);
        BlockScene::instance()->addItem(third);

    } else {
        Layout *horLayout = createLayoutForInsert( parent, "aux_line");
        parent->insertBehind(_item, horLayout);
        parent->removeElement(_item);

        second = createItemForInsert( newChar, horLayout);
        third = createItemForInsert( !newChar.isSpace(), text.mid(pos+1), horLayout);

        horLayout->insertOnStart(_item);
        horLayout->insertBehind(_item, second);
        horLayout->insertBehind(second, third);

        horLayout->updateChildNeighbors();
        BlockScene::instance()->addItem(horLayout);
        BlockScene::instance()->addItem(second);
        BlockScene::instance()->addItem(third);
    }

    second->setFocus();
    QTextCursor cursor = second->textCursor();
    cursor.movePosition(QTextCursor::End);
    second->setTextCursor(cursor);
    // TODO
    ////    _item->edited(second);
    ////    second->edited(second);
    ////    third->edited(second);
    AbstractElement *parseble = prepareParsable(parent, second);
    parseble->edited();
}


void WriteItemCommand::simpleAdditionStartEnd(QChar newChar, bool inStart) {
    qDebug() << ( inStart ? "add at start  new Item" : "add at end  new item");
    AbstractElement *parsable = NULL;
    undoSimpleAddition();
    Layout *parent = _item->getLayoutParrent();
    Item *neighbor = dynamic_cast<Item*>(_item->nextPreviousAlsoHor( !inStart));

    if (NULL != neighbor && neighbor->state()->isSpaced() == newChar.isSpace()) {
        qDebug() << ( inStart ? "merge with previous" : "merge with next");

        neighbor->setFocus();
        neighbor->blockSignals(true);
        QTextCursor cursor = neighbor->textCursor();
        cursor.beginEditBlock();
        if (inStart) {
            cursor.movePosition(QTextCursor::End);
        } else {
            cursor.movePosition(QTextCursor::Start);
        }
        cursor.insertText(QString(newChar));
        cursor.endEditBlock();
        neighbor->setTextCursor(cursor);
        neighbor->blockSignals(false);

        parsable = prepareParsable(neighbor);
        parsable->edited();
        //// neighbor->edited(neighbor);
    } else {
        qDebug() << "no merge";
        Item *newItem = NULL;

        if (parent->orientation() == Qt::Horizontal) {
            newItem = createItemForInsert( newChar);
            if (inStart) {
                parent->insertBefore(_item, newItem);
            } else {
                parent->insertBehind(_item, newItem);
            }
        } else {
            Layout *horLayout = createLayoutForInsert( parent, "aux_line");

            parent->insertBehind(_item, horLayout);
            parent->removeElement(_item);
            if (inStart) {
                newItem = createItemForInsert(newChar, horLayout);
                horLayout->insertOnStart(newItem);
                horLayout->insertBehind(newItem, _item);
            } else {
                horLayout->insertOnStart(_item);
                newItem = createItemForInsert(newChar, horLayout);
                horLayout->insertBehind(_item, newItem);
            }
            parent->updateChildNeighbors();
            BlockScene::instance()->addItem(horLayout);
        }
        BlockScene::instance()->addItem(newItem);
        ///// newItem->edited(newItem);
        parsable = prepareParsable(newItem);
        parsable->edited();
    }
}


Item *WriteItemCommand::createItemForInsert(QChar newChar) {
    return createItemForInsert(newChar.isSpace(), QString(newChar));
}

Item *WriteItemCommand::createItemForInsert(QChar newChar, Layout *parent) {
    return createItemForInsert(newChar.isSpace(), QString(newChar), parent);
}

Item *WriteItemCommand::createItemForInsert(bool stable, QString text) {
    return createItemForInsert(stable, text, _item->getLayoutParrent());
}

Item *WriteItemCommand::createItemForInsert(bool stable, QString text, Layout *parent) {
    if (stable) {
        return Parser::instance()->createStableItem(parent, text);
    } else {
        //return Parser::instance()->createChangedItem(_item->getLayoutParrent(), text);
        return Parser::instance()->createNewItem(parent, "changed_text", text);
    }
}

Layout *WriteItemCommand::createLayoutForInsert(Layout *parent, QString type) {
    return Parser::instance()->createNewLayout(parent, type);
}

void WriteItemCommand::undoSimpleAddition() {
    _item->blockSignals(true);
    // undo
    if (0 == pos) {
        _item->setPlainText(_item->toPlainText().mid(1));
    } else {
        //_item->document()->undo(&cursor);
        QTextCursor cursor(_item->textCursor());
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        _item->setTextCursor(cursor);
    }
    _item->blockSignals(false);
}

AbstractElement *WriteItemCommand::findInsteadtoSelect() {
    AbstractElement *instead = _item->nextPreviousAlsoHor(false);
    if (NULL == instead) {
        instead = _item->nextPreviousAlsoHor(true);
    }
    if (NULL == instead) {
        instead = _item->nextPreviousAlsoVert(false);
    }
    if (NULL == instead) {
        instead = _item->nextPreviousAlsoVert(true);
    }
    return instead;
}

AbstractElement *WriteItemCommand::prepareParsable(Item *item) const {
    AbstractElement *parseble = item->getSecondParsable();
    parseble->setCurPos( item->cursorPositionIn(parseble));
    return parseble;
}

AbstractElement *WriteItemCommand::prepareParsable(AbstractElement *element, Item *focusItem) const {
    AbstractElement *parseble = element->getSecondParsable();
    parseble->setCurPos( focusItem->cursorPositionIn(parseble));
    return parseble;
}


AbstractElement *WriteItemCommand::prepareParsableForRemoved(AbstractElement *toRemove, Item *focusItem) const {
    //qDebug() << "getParsableForRemoved";
    AbstractElement *right = toRemove->nextPreviousAlsoVert(true);
    while (right && right->state()->isSpaced()) {
        right = right->nextPreviousAlsoVert(true);
    }
    //qDebug() << "finded right";
    AbstractElement *left = toRemove->nextPreviousAlsoVert(false);
    while (left && left->state()->isSpaced()) {
        left = left->nextPreviousAlsoVert(false);
    }
    //qDebug() << "finded left";
    AbstractElement *parseble = toRemove;
    if (right && left) {
        //qDebug() << " find PM for right&left";
        parseble = left->findMutualParent(right);
    } else  if (left) {
        //qDebug() << " find PM for left";
        parseble = toRemove->findMutualParent(left);
    } else  if (right) {
        //qDebug() << " find PM for right";
        parseble = toRemove->findMutualParent(right);
    }
    //qDebug() << " mutual paren is finded";
    parseble = parseble->getSecondParsable();
    parseble->setCurPos( focusItem->cursorPositionIn(parseble));
    return parseble;
}

bool WriteItemCommand::hasSpace(const QString str) const {
    QString::ConstIterator it;
    for (it = str.constBegin(); it != str.constEnd(); it++) {
        if (it->isSpace()) {
            return true;
        }
    }
    return false;
}



void WriteItemCommand::complexAddition() {

}

void WriteItemCommand::undo() {
    qDebug() << "UNDO writeItemCommand ";
}

