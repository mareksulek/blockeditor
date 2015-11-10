#include "include/minimapview.h"
#include "include/scene/blockscene.h"

MiniMapView::MiniMapView()
{
    setAlignment(Qt::AlignTop|Qt::AlignLeft);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setInteractive(false);
}

MiniMapView::~MiniMapView()
{
}

void MiniMapView::updateScene(const QList<QRectF> &rects)
{
    BlockScene* bs = dynamic_cast<BlockScene *>(scene());
    fitInView(bs->sceneRect(), Qt::KeepAspectRatio);
    setSceneRect(bs->_root->childrenBoundingRect().translated(20,0));
}
