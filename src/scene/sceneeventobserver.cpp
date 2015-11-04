#include "scene/sceneeventobserver.h"

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

#include "scene/command/command.h"
#include "scene/command/cursormovecommand.h"
#include "scene/command/selectitemcommand.h"
#include "scene/command/changemarkedcommand.h"
#include "scene/command/moveelementcommand.h"
#include "scene/command/dragelemententercommand.h"
#include "scene/command/dragelementleavecommand.h"
#include "scene/command/dropelementcommand.h"
#include "scene/blockscene.h"

SceneEventObserver::SceneEventObserver(QGraphicsItem *parent)
    : QGraphicsItem(parent) {

}


bool SceneEventObserver::sceneEventFilter ( QGraphicsItem * watched, QEvent *event ) {
    Command *command = NULL;

    if (event->type() == QEvent::GraphicsSceneWheel) {
        qDebug() << "SEFO  MOUSE wheel EVENT";
        QGraphicsSceneWheelEvent * wheelEvent = static_cast<QGraphicsSceneWheelEvent *>(event);
        if (Qt::ControlModifier & wheelEvent->modifiers()) {
            command = new ChangeMarkedCommand(watched, wheelEvent);
        }
    }
    else if (event->type() == QEvent::GraphicsSceneMousePress) {
        qDebug() << "SEFO  MOUSE press EVENT";
        QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
        if (mouseEvent->buttons() & Qt::LeftButton) {
            command = new SelectItemCommand(watched, mouseEvent);
        }
    }
    else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
        //QGraphicsSceneMouseEvent * mp = static_cast<QGraphicsSceneMouseEvent *>(event);
        qDebug() << "SEFO  MOUSE release EVENT";
    }
    else if (event->type() == QEvent::GraphicsSceneMouseMove) {
        qDebug() << "SEFO  MOUSE move EVENT";
        QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
        if (Qt::ControlModifier & mouseEvent->modifiers()) {
            command = new MoveElementCommand(watched, mouseEvent);
        }
    }

    else if (event->type() == QEvent::GraphicsSceneDragEnter) {
        qDebug() << "SEFO  MOUSE drag enter EVENT";
        QGraphicsSceneDragDropEvent * dragDropEvent = static_cast<QGraphicsSceneDragDropEvent *>(event);
        command = new DragElementEnterCommand(watched, dragDropEvent);
    }
//    else if (event->type() == QEvent::GraphicsSceneDragLeave) {
//        qDebug() << "SEFO  MOUSE drag leave EVENT";
//        QGraphicsSceneDragDropEvent * dragDropEvent = static_cast<QGraphicsSceneDragDropEvent *>(event);
//        command = new DragElementLeaveCommand(watched, dragDropEvent);
//    }
    else if (event->type() == QEvent::GraphicsSceneDrop) {
        qDebug() << "SEFO  MOUSE drop EVENT";
        QGraphicsSceneDragDropEvent * dragDropEvent = static_cast<QGraphicsSceneDragDropEvent *>(event);
        command = new DropElementCommand(watched, dragDropEvent);
    }

    else if (event->type() == QEvent::KeyPress) {
        qDebug() << "SEFO KEY press EVENT " << event;
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        command = keyCommand( watched, keyEvent);
    }

    if (NULL == command) {
        return false;
    }
    BlockScene::instance()->addCommand(command);
    return true;
}



// Key Command
Command *SceneEventObserver::keyCommand(QGraphicsItem *watched, QKeyEvent *event) {

    if ( event->key() == Qt::Key_Left  ||
         event->key() == Qt::Key_Right ||
         event->key() == Qt::Key_Up    ||
         event->key() == Qt::Key_Down ) {
        return new CursorMoveCommand(watched, event);
    }
   return NULL;
}
