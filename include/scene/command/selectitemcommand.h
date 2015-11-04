#ifndef SELECTITEMCOMMAND_H
#define SELECTITEMCOMMAND_H


#include "scene/command/command.h"


class QGraphicsSceneMouseEvent;
class QGraphicsItem;
class Item;


class SelectItemCommand : public Command {
public:
    explicit SelectItemCommand(QGraphicsItem *watched, QGraphicsSceneMouseEvent *event);

    void execute();
    void undo();

protected:


   QGraphicsSceneMouseEvent *_event;
   Item *_item;



};

#endif
