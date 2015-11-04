#include "scene/command/selectitemcommand.h"
#include <QDebug>


#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>


//#include "item/layout.h"
//#include "item/abstractelement.h"
//#include "style/style.h"
#include "scene/blockscene.h"
#include "scene/scenestate.h"
#include "item/item.h"


SelectItemCommand::SelectItemCommand(QGraphicsItem *watched, QGraphicsSceneMouseEvent *event)
    : Command()
{
    _event = event;
    _item = dynamic_cast<Item *>(watched) ;


}


void SelectItemCommand::execute() {
    qDebug() << "EXE selectItemCommand";

    SceneState *state = BlockScene::instance()->getSceneState();

    if (_item != state->getSelectedItem()) {
        //qDebug() << "new selected";
        state->setSelectedItem(_item);

        AbstractElement *painted = state->getPaintedElement();
        if (!painted || !_item->isParent(painted)) {
            if(painted) {
                state->getPaintedElement()->setPaintEnable(false);
            }
            state->setPaintedElement(dynamic_cast<AbstractElement*>(_item));
        }
    }
    qDebug() << _item->toString();

    _item->mousePressEvent(_event);

}



void SelectItemCommand::undo() {
    qDebug() << "UNDO selectItemCommand ";
}

