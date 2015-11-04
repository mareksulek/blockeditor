#include "scene/blockscene.h"
#include "QDebug"

#include <QGraphicsLinearLayout>
#include <QGraphicsLayoutItem>
#include <QGraphicsWidget>
#include <QLabel>

#include "scene/sceneeventobserver.h"
#include "scene/scenestate.h"

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
    _selectedLeaf = NULL;
    _paintedElemement = NULL;
    //test();
    _sceneState = new SceneState;
    _eventFilter = new SceneEventObserver;
    addItem(_eventFilter);
}

void BlockScene::addItem(QGraphicsItem *item) {
    qDebug() << "A  ";
    QGraphicsScene::addItem(item);
}

Item *BlockScene::addParserItem(Item *item)
{
    qDebug() << "-- added item " << item->toPlainText() << "   " << item->getType();

    if( item->getLayoutParrent() == NULL){
        //addItem(item);
        qDebug() << "also in scene";

    } else {
        qDebug() << "      with parrent: " << item->getLayoutParrent()->getType();

        item->getLayoutParrent()->addItem(item);
        //item->getLayoutParrent()->activate();
        //item->getLayoutParrent()->updateGeometry();
        //qDebug() << "islayout" << item->getLayoutParrent()->isLayout();

        //Layout *parrent = dynamic_cast<Layout*>( item->getLayoutParrent());
        //parrent->addLayoutChild(item);

    }
    addItem( item);
    item->installSceneEventFilter( _eventFilter);

QConnect:connect( item->_document, SIGNAL(contentsChanged()), _root, SLOT(childItemChanged()));

    return item;
}

Layout* BlockScene::addParserLayout( Layout *layout) {
    qDebug() << "-- added layout " << layout->getType();
    if( layout->parentLayoutItem() == NULL){
        qDebug() << "also in scene";
        _form->setLayout(layout);
        _root = layout;
        _root->setParrentE(NULL);
    } else {
        qDebug() << "      with parrent: " << layout->getLayoutParrent()->getType();
        //Layout *parrent = dynamic_cast<Layout*>( layout->getLayoutParrent());
        //parrent->addLayoutChild(layout);
        layout->getLayoutParrent()->addItem(layout);

    }
    addItem( layout);
    QConnect:connect(this, SIGNAL(activeContextChanged(Layout*)), layout, SLOT(activeContextChanged(Layout*)));
    return layout;
}

void BlockScene::updateTreeNeighbors()
{
    if (NULL == _root) {
        qDebug() << "Warning: BlockScene::updateTreeNeighbors(): _root is null.";
        return;
    }
    _root->updateChildNeighbors();

}

void BlockScene::setContext(Layout *layout)
{
    if(_context!=layout){
        _context = layout;
        emit activeContextChanged(layout);
        update();
    }
}

void BlockScene::unsetContext()
{
    setContext(_root);
}

void BlockScene::test()
{
    /*
    Item *i1 =  new Item("-1-", "-");
    Item *i2 =  new Item("-2-", "-");
    Item *i3 =  new Item("-3-", "-");
    Item *i4 =  new Item("-4-", "-");
    Item *i5 =  new Item("-5-", "-");
    Item *i6 =  new Item("-6-", "-");
    Item *i7 =  new Item("-7-", "-");
    Item *i8 =  new Item("-8-", "-");
    Item *i9 =  new Item("-9-", "-");
    Item *i10 =  new Item("-10-", "-");


    QGraphicsLinearLayout *v1 = new QGraphicsLinearLayout(Qt::Vertical);
    QGraphicsLinearLayout *vv1 = new QGraphicsLinearLayout(Qt::Vertical, v1);
    QGraphicsLinearLayout *hh2 = new QGraphicsLinearLayout(Qt::Horizontal, v1);
    QGraphicsLinearLayout *vv3 = new QGraphicsLinearLayout(Qt::Vertical, v1);
    QGraphicsLinearLayout *vv4 = new QGraphicsLinearLayout(Qt::Vertical, vv1);
    QGraphicsLinearLayout *vv5 = new QGraphicsLinearLayout(Qt::Vertical, vv1);
    QGraphicsLinearLayout *vv6 = new QGraphicsLinearLayout(Qt::Vertical, vv4);


    v1->addItem(i1);
   v1->addItem(vv1);
  vv1->addItem(vv4);
  vv1->addItem(vv5);
  vv4->addItem(vv6);
  v1->addItem(hh2);
  v1->addItem(vv3);


    v1->addItem(i2);
    vv1->addItem(i3);
    vv4->addItem(i4);
    vv5->addItem(i5);
    vv6->addItem(i6);
    v1->addItem(i7);
    hh2->addItem(i8);
    hh2->addItem(i9);
    vv3->addItem(i10);
    _form->setLayout(v1);

    addItem(i1);
    addItem(i2);
    addItem(i3);
    addItem(i4);
    addItem(i5);
    addItem(i6);
    addItem(i7);
    addItem(i8);
    addItem(i9);
    addItem(i10);

    qDebug() << "test" << vv4->isLayout();
*/
}
