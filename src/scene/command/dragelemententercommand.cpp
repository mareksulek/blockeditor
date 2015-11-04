#include "scene/command/dragelemententercommand.h"
#include <QDebug>


#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsItem>


//#include "item/layout.h"
//#include "item/abstractelement.h"
//#include "style/style.h"
//#include "scene/blockscene.h"
//#include "scene/scenestate.h"
#include "item/item.h"
#include "scene/mimedata.h"


DragElementEnterCommand::DragElementEnterCommand(QGraphicsItem *watched, QGraphicsSceneDragDropEvent *event)
    : Command()
{
    _event = event;
    _item = dynamic_cast<Item *>(watched) ;


}


void DragElementEnterCommand::execute() {
    qDebug() << "EXE dragElementEnterCommand";

    //SceneState *state = BlockScene::instance()->getSceneState();

    const MimeData *md = dynamic_cast<const MimeData*>(_event->mimeData());
    if ( md && md->hasElement()) {
       /////--- //Item *item = dynamic_cast<Item *>(md->element());
       qDebug() << "drag " << _item->toPlainText();


       //_item->dragEnterEvent(_event);
        _event->setAccepted(true);
        //dragOver = true;
        //_item->update();
    } else {
        _event->setAccepted(false);
    }

}

void DragElementEnterCommand::undo() {
    qDebug() << "UNDO dragElementEnterCommand ";
}

