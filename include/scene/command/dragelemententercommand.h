#ifndef DRAGELEMENTENTERCOMMAND_H
#define DRAGELEMENTENTERCOMMAND_H


#include "scene/command/command.h"


class QGraphicsSceneDragDropEvent;
class QGraphicsItem;
class Item;


class DragElementEnterCommand : public Command {
public:
    explicit DragElementEnterCommand(QGraphicsItem *watched, QGraphicsSceneDragDropEvent *event);

    void execute();
    void undo();

protected:


   QGraphicsSceneDragDropEvent *_event;
   Item *_item;



};

#endif
