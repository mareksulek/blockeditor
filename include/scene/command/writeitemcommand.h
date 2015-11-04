#ifndef WRITEITEMCOMMAND_H
#define WRITEITEMCOMMAND_H


#include "scene/command/command.h"

class QGraphicsItem;
class AbstractElement;
class Item;
class Layout;

class WriteItemCommand : public Command {
public:
    explicit WriteItemCommand(QGraphicsItem *watched, int pos, int charsRemoved, int charsAdded);

    void execute();
    void undo();

protected:

    bool hasSpace(const QString str) const;

    void simpleAddition();
    void simpleRemove();
    void complexAddition();
    void undoSimpleAddition();
    void simpleAdditionStartEnd(QChar newChar, bool inStart);
    void simpleAdditionMiddle(QChar newChar);
    void simpleAdditionEnter();

    //AbstractElement *WriteItemCommand::getParsable(Item *left, Item *right, Item *focused) const;
    AbstractElement *prepareParsable(Item *item) const;
    AbstractElement *prepareParsable(AbstractElement *element, Item *focusItem) const;
    AbstractElement *prepareParsableForRemoved(AbstractElement *toRemove, Item *focusItem) const;

    AbstractElement *findInsteadtoSelect();
    Item *createItemForInsert(QChar newChar);
    Item *createItemForInsert(QChar newChar, Layout *parent);
    Item *createItemForInsert(bool stable, QString text);
    Item *createItemForInsert(bool stable, QString text, Layout *parent);
    Layout *createLayoutForInsert(Layout *parent, QString type);


    Item *_item;
    int pos;
    int charsRemoved;
    int charsAdded;



};

#endif
