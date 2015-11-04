#include "scene/blockscene.h"
#include "QDebug"

#include <QGraphicsLinearLayout>
#include <QGraphicsLayoutItem>
#include <QGraphicsWidget>
#include <QLabel>

#include "scene/reparser.h"
#include "scene/sceneeventobserver.h"
#include "scene/scenestate.h"
#include "scene/command/command.h"

BlockScene *BlockScene::inst = 0;

BlockScene *BlockScene::instance( QObject *parent)
{
    if (!inst) {
        inst = new BlockScene(parent);
    }
    return inst;
}


BlockScene::BlockScene( QObject *parent)
    : QGraphicsScene(parent)
{
    //setSceneRect(0, 0, 300, 300);
    _form = new QGraphicsWidget;
    addItem(_form);

    _sceneState = new SceneState;
    _reparser = new Reparser();
    _eventFilter = new SceneEventObserver;
    addItem(_eventFilter);

}

Layout *BlockScene::root() const {
    return _root;
}

void  BlockScene::setRoot(Layout *root) {
    _root = root;
    _root->setLayoutParrent(NULL);
    _root->setNext(NULL);
    _root->setPrevius(NULL);
    //_form->setLayout(_root);
    //_form->layoutChanged();
}

void BlockScene::addItem(QGraphicsItem *graphicItem, bool recursive) {
    //qDebug() << "A  ";
    QGraphicsScene::addItem(graphicItem);

    Item   *item = dynamic_cast<Item *>( graphicItem);
    Layout *layout = dynamic_cast<Layout *>( graphicItem);
    if (item) {
        item->attach(_reparser);
        graphicItem->installSceneEventFilter( _eventFilter);
QConnect:connect( item->document(), SIGNAL(contentsChanged()), _root, SLOT(childItemChanged()));
    }

    if (layout) {
        layout->attach(_reparser);
    }

    if (recursive && layout) {
        for (int i=0; i < layout->count(); i++) {
            QGraphicsItem *child = dynamic_cast<QGraphicsItem*>(layout->itemAt(i));
            if (child) {
                addItem(child, recursive);
            }
        }
    }
}

void BlockScene::removeItem(QGraphicsItem *graphicItem, bool recursive) {

    graphicItem->removeSceneEventFilter(_eventFilter);
    AbstractElement *element = dynamic_cast<AbstractElement *>( graphicItem);
    if (element) {
        element->detach(_reparser);

    }


    if (recursive) {
        Layout *layout = dynamic_cast<Layout*>( graphicItem);
        if (layout) {
            for (int i=0; i < layout->count(); i++) {
                QGraphicsItem *child = dynamic_cast<QGraphicsItem*>(layout->itemAt(i));
                if (child) {
                    removeItem(child, recursive);
                }
            }
        }
    }
    QGraphicsScene::removeItem(graphicItem);
}


void BlockScene::addNewRoot(Layout *root) {


    _form->setLayout(root);
    _root = root;
    _root->updateChildNeighbors();
    addItem(_root, true);
    qDebug() << "elements count="<<_root->elementCount();

}


void BlockScene::addCommand(Command *command)
{
    command->execute();
    _commandStack.push( command);
}

