#ifndef DRAGELEMENTLEAVECOMMAND_H
#define DRAGELEMENTLEAVECOMMAND_H


#include "scene/command/command.h"


class QGraphicsSceneDragDropEvent;
class QGraphicsItem;
class Item;


class DragElementLeaveCommand : public Command {
public:
    explicit DragElementLeaveCommand(QGraphicsItem *watched, QGraphicsSceneDragDropEvent *event);

    void execute();
    void undo();

protected:


   QGraphicsSceneDragDropEvent *_event;
   Item *_item;



};

#endif
