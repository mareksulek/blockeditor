#ifndef CHANGEMARKEDCOMMAND_H
#define CHANGEMARKEDCOMMAND_H


#include "scene/command/command.h"


class QGraphicsSceneWheelEvent;
class QGraphicsItem;
class Item;


class ChangeMarkedCommand : public Command {
public:
    explicit ChangeMarkedCommand(QGraphicsItem *watched, QGraphicsSceneWheelEvent *event);

    void execute();
    void undo();

protected:


   QGraphicsSceneWheelEvent *_event;
   Item *_item;



};

#endif
