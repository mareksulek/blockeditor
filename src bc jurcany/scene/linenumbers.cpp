#include "scene/linenumbers.h"
#include <cmath>

LineNumbers::LineNumbers(BlockScene *scene)
{
    _scene = scene;
    QObject::connect(scene, &BlockScene::changed, this, &LineNumbers::updateLines);
}

LineNumbers::~LineNumbers()
{

}

QRectF LineNumbers::boundingRect() const
{
    if(_lines.empty()){
        return QRectF();
    }else{
        return QRectF(0, _lines.front(), ceil(log10(_lines.size()+1))*9+9, _lines.back()+20);
    }
}

void LineNumbers::updateLines(const QList<QRectF> &region)
{
    QList<QGraphicsItem *> items = _scene->items();
    QList<QGraphicsItem *>::iterator iter = items.begin();
    //qDebug() << "LineNumbers updating...";

    _lines.clear();
    QGraphicsItem* item;
    while(iter!=items.end()){
        item = *iter;
        if(item->pos().x()>5 && item->pos().y()>5){
            _lines.push_back((*iter)->pos().y()+1);
        }
        iter++;
    }
    std::sort(_lines.begin(), _lines.end());
    std::vector<int>::iterator it;
    it = std::unique(_lines.begin(), _lines.end());
    _lines.resize(it-_lines.begin());
}

void LineNumbers::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //qDebug() << "LineNumbers painted...";

    std::vector<int>::iterator iter = _lines.begin();
    QStaticText linenumber_label;
    while(iter!=_lines.end()){
        QRect rect = QRect(0, *iter, boundingRect().width(), 20);
        painter->setPen(QColor(200,200,200));
        painter->fillRect(rect, QBrush(QColor(70,70,70)));
        painter->drawRect(rect);
        linenumber_label.setText(QString::number(iter-_lines.begin()+1));
        painter->drawStaticText(rect.right()-4-linenumber_label.size().width(), (*iter)+4, linenumber_label);
        iter++;
    }
}
