#ifndef CURSORMOVECOMMAND_H
#define CURSORMOVECOMMAND_H


#include "scene/command/command.h"

class QGraphicsItem;
class QKeyEvent;
class Item;
class AbstractElement;

class CursorMoveCommand : public Command {
public:
    explicit CursorMoveCommand(QGraphicsItem *watched, QKeyEvent *event);

    void execute();
    void undo();

protected:
    void horCursorMovement(bool toNext);
    void verCursorMovement(bool down);
    int skipChild(AbstractElement *child, int linePos);

    Item *_item;
    QKeyEvent *_event;



};

#endif
