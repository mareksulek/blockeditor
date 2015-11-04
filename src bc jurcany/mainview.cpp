#include "include/mainview.h"
#include "scene/blockscene.h"

MainView::MainView()
    : QGraphicsView(), _lineNumbersOverlay(NULL), _contextOverlay(NULL)
{
    setAlignment(Qt::AlignTop|Qt::AlignLeft);

}

MainView::~MainView()
{

}

void MainView::setScene(QGraphicsScene *scene)
{
    if(BlockScene *bs = dynamic_cast<BlockScene *>(scene)){
        QGraphicsView::setScene(scene);
        if(!_lineNumbersOverlay){
            _lineNumbersOverlay = new QGraphicsScene();
            LineNumbers *ln = new LineNumbers(bs);
            _lineNumbersOverlay->addItem(ln);
            connect(_lineNumbersOverlay, SIGNAL(changed(QList<QRectF>)), SLOT(overlayChanged()));
        }
        if(!_contextOverlay){
            _contextOverlay = new QGraphicsScene();
            connect(bs, SIGNAL(activeContextChanged(Layout*)), this, SLOT(contextChanged()));
        }
    }
}

void MainView::updateScene(const QList<QRectF> &rects)
{
    QGraphicsView::updateScene(rects);
    BlockScene* bs = dynamic_cast<BlockScene *>(scene());
    QRectF r = bs->_root->childrenBoundingRect();
    r.translate(20,0);
    r.translate(-1*_lineNumbersOverlay->itemsBoundingRect().width(),0);
    setSceneRect(r);
}

void MainView::contextChanged()
{
    BlockScene *bs = dynamic_cast<BlockScene *>(scene());
    _contextOverlay->clear();
    if(bs->context() && bs->context()->getType()!="root"){
        qDebug() << "DEBUG_CONTEXT :: " << bs->context()->toString();
        _contextOverlay->addRect(0,0,bs->width(),bs->context()->boundingRect().top()+16, Qt::NoPen, QBrush(QColor(255,255,255,230)));
        _contextOverlay->addRect(0,bs->context()->boundingRect().bottom()-1,bs->width(),viewport()->height(), Qt::NoPen, QBrush(QColor(255,255,255,230)));
    }
}

void MainView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    if(_contextOverlay) paintContextOverlay();
    if(_lineNumbersOverlay) paintLineNumbersOverlay();
}

void MainView::paintLineNumbersOverlay()
{
    QPainter p(viewport());
    p.setRenderHints(renderHints());
    QRectF r = _lineNumbersOverlay->itemsBoundingRect().adjusted(0,0,1,0);
    _lineNumbersOverlay->render(&p, r.translated(0,mapFromScene(0,0).y()), r);
    scene()->update();
}

void MainView::paintContextOverlay()
{
    QPainter p(viewport());
    p.setRenderHints(renderHints());
    QRectF r = _contextOverlay->itemsBoundingRect();
    _contextOverlay->render(&p, r.translated(0,mapFromScene(0,0).y()).adjusted(0,0,viewport()->width(),0), r);
    scene()->update();
}

