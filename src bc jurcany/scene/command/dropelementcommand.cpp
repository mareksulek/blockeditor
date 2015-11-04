#include "scene/command/dropelementcommand.h"
#include <QDebug>


#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsItem>
#include <QGraphicsLayoutItem>



//#include "style/style.h"
//#include "scene/blockscene.h"
//#include "scene/scenestate.h"
#include "item/item.h"
#include "item/layout.h"
#include "item/abstractelement.h"
#include "scene/mimedata.h"


DropElementCommand::DropElementCommand(QGraphicsItem *watched, QGraphicsSceneDragDropEvent *event)
    : Command()
{
    _event = event;
    _item = dynamic_cast<Item *>(watched) ;


}


void DropElementCommand::execute() {
    qDebug() << "EXE dropElementCommand";

    //SceneState *state = BlockScene::instance()->getSceneState();

    //dragOver = false;
    const MimeData *md = dynamic_cast<const MimeData*>(_event->mimeData());
    if ( md && md->hasElement()) {
        qDebug() << "drop 1 ";
        AbstractElement *item = dynamic_cast<AbstractElement *>(md->element());
        qDebug() << "drop 2" << item->textE() << " "  << item;


        // remove from old position

        // TODO item parrent can be null -> null pointer exception
        item->getLayoutParrent()->removeItem(dynamic_cast<QGraphicsLayoutItem*>(item));
        item->getLayoutParrent()->invalidate();  // TODO remove maybe
        //item->getLayoutParrent()->updateGeometry();
        //item->getLayoutParrent()->getLayoutParrent()->invalidate();
        //item->getLayoutParrent()->getLayoutParrent()->updateGeometry();

        //item->getLayoutParrent()->setGeometry();
        if( item->getNext()) {
            item->getNext()->setPrevius(item->getPrevius());
        }
        if( item->getPrevius()) {
            item->getPrevius()->setNext(item->getNext());
        }
        // insert to new position
        Layout *parrent = dynamic_cast <Layout*>( _item->getLayoutParrent());
        if(parrent) {
            item->setLayoutParrent(parrent);
            //item->setParentLayoutItem(parrent);
            int index = parrent->indexOf(_item);
            parrent->insertItem(index+1,dynamic_cast<QGraphicsLayoutItem*>(item));


            item->setNext(_item->getNext());
            item->setPrevius(_item);
            if( _item->getNext()) {
                _item->getNext()->setPrevius(item);
            }
            _item->setNext(item);

            //parrent->updateGeometry();
        }

        _event->setDropAction(Qt::MoveAction);
        _event->accept();
        // event->acceptProposedAction();
    }
    _item->update();


}



