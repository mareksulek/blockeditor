#include "scene/command/moveelementcommand.h"
#include <QDebug>


#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QDrag>



//#include "item/abstractelement.h"
#include "scene/blockscene.h"
#include "scene/scenestate.h"
#include "item/item.h"
#include "scene/mimedata.h"



MoveElementCommand::MoveElementCommand(QGraphicsItem *watched, QGraphicsSceneMouseEvent *event)
    : Command()
{
    _event = event;
    _item = dynamic_cast<Item *>(watched) ;
}


void MoveElementCommand::execute() {
    qDebug() << "EXE moveElementCommand";

    SceneState *state = BlockScene::instance()->getSceneState();


    if (QLineF(_event->screenPos(), _event->buttonDownScreenPos(Qt::LeftButton))
            .length() < QApplication::startDragDistance()) {
        return;
    }

    AbstractElement *movedElement = state->getPaintedElement();
    QDrag *drag = new QDrag(_event->widget());
    QMimeData *mime = new MimeData(movedElement);
    drag->setMimeData(mime);
    mime->setText("textdata");

    drag->setPixmap( movedElement->toPixmap());
    drag->setHotSpot(QPoint(0, 0));

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

    if (dropAction == Qt::MoveAction) {
        qDebug() << "move exec done";
        Layout *parrent = _item->getLayoutParrent();
        //        if(parrent) {
        //            parrent->updateGeometry();
        //        }
    }
}

void MoveElementCommand::undo() {
    qDebug() << "UNDO moveElementCommand ";
}


