#include "item/standardelementbuilder.h"
#include <QDebug>

//#include <QGraphicsTextItem>
//#include <QGraphicsLayoutItem>

#include "item/abstractelement.h"
#include "item/item.h"
#include "item/layout.h"
#include "style/styleutil.h"
#include "style/style.h"
#include "item/state/elementstable.h"
#include "item/state/elementvalid.h"

StandardElementBuilder::StandardElementBuilder()
    :  ElementBuilder()
{
    _element = NULL;

}


void StandardElementBuilder::buildItem(Layout *parent, QString text) {
    _element = new Item(parent, text);
}

void StandardElementBuilder::buildLayout(Layout *parent) {
    _element = new Layout(parent);
}

void StandardElementBuilder::buildStyle() {
    _element->setStyleE( StyleUtil::instance()->getStyle(_elementType));
}

void StandardElementBuilder::buildState() {
    _element->setState( new ElementValid);
}

void StandardElementBuilder::buildState(ElementState *state) {
    _element->setState( state);
}

void StandardElementBuilder::buildType(QString type) {
    _elementType = type;
    _element->setType(type);
}

AbstractElement*  StandardElementBuilder::getElement() {
    return _element;
}
