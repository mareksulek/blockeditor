#ifndef DROPELEMENTCOMMAND_H
#define DROPELEMENTCOMMAND_H


#include "scene/command/command.h"


class QGraphicsSceneDragDropEvent;
class QGraphicsItem;
class Item;


class DropElementCommand : public Command {
public:
    explicit DropElementCommand(QGraphicsItem *watched, QGraphicsSceneDragDropEvent *event);

    void execute();
    void undo();

protected:


   QGraphicsSceneDragDropEvent *_event;
   Item *_item;



};

#endif
