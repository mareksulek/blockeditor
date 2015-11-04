#include "scene/command/changemarkedcommand.h"
#include <QDebug>


#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>

#include "item/item.h"
#include "scene/scenestate.h"
#include "scene/blockscene.h"

//#include "item/layout.h"
//#include "item/abstractelement.h"
//#include "style/style.h"


ChangeMarkedCommand::ChangeMarkedCommand(QGraphicsItem *watched, QGraphicsSceneWheelEvent *event)
    : Command()
{
    _event = event;
    _item = dynamic_cast<Item *>(watched) ;


}


void ChangeMarkedCommand::execute() {
    qDebug() << "EXE changeMarkedCommand ";

    SceneState *state = BlockScene::instance()->getSceneState();
    AbstractElement *painted = state->getPaintedElement();

//    qDebug() << "  ACT : "<< _item->toString();
//    if (state->getSelectedItem()) {
//        qDebug() << "    OLD SELECTED: "<< state->getSelectedItem()->toString();
//        qDebug() << "    OLD PAINTED:  "<< state->getPaintedElement()->toString();
//    }
    if (_item != state->getSelectedItem()) {
        // qDebug() << "new selected";
        state->setSelectedItem(_item);

        if (!painted || !_item->isParent(painted)) {
            if(painted) {
                state->getPaintedElement()->setPaintEnable(false);
            }
            state->setPaintedElement(dynamic_cast<AbstractElement*>(_item));
        }
    }


    painted = state->getPaintedElement();

    if( _event->delta() > 0) {  // UP
        //qDebug() << "UP";
        AbstractElement *parentE = dynamic_cast<AbstractElement*>(painted->getLayoutParrent());

        if(parentE) {
            parentE->setPaintEnable(true);
            painted->setPaintEnable(false);
            state->setPaintedElement( parentE);
        }

    } else {  // DOWN
        AbstractElement *child = dynamic_cast<AbstractElement*>(_item);

        if( painted != child ) {
            //qDebug() << "DOWN";
            AbstractElement *parentE = dynamic_cast<AbstractElement*>(_item->getLayoutParrent());

            while(parentE) {
                if (parentE == painted) {
                    painted->setPaintEnable(false);
                    child->setPaintEnable(true);
                    state->setPaintedElement(child);
                    return;
                }
                child = parentE;
                parentE = dynamic_cast<AbstractElement*>( parentE->getLayoutParrent());
            }
        }
    }
    //qDebug() << "    NEW PAINTED:  "<< state->getPaintedElement()->toString();
}

void ChangeMarkedCommand::undo() {
    qDebug() << "UNDO changeMarkedCommand ";
}

