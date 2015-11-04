#include "scene/command/dragelementleavecommand.h"
#include <QDebug>


#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsItem>


//#include "item/layout.h"
//#include "item/abstractelement.h"
//#include "style/style.h"
//#include "scene/blockscene.h"
//#include "scene/scenestate.h"
#include "item/item.h"


DragElementLeaveCommand::DragElementLeaveCommand(QGraphicsItem *watched, QGraphicsSceneDragDropEvent *event)
    : Command()
{
    _event = event;
    _item = dynamic_cast<Item *>(watched) ;


}


void DragElementLeaveCommand::execute() {
    qDebug() << "EXE dragElementLeaveCommand";

    qDebug() << "drag-leave " << _item->toPlainText();

    //SceneState *state = BlockScene::instance()->getSceneState();

    //dragOver = false;

    //_item->update();
}



