#include "item/state/elementchanged.h"
#include <QDebug>

#include "item/abstractelement.h"
#include "item/item.h"
#include "item/layout.h"
#include "style/style.h"
#include "scene/parser.h"
#include "scene/blockscene.h"
#include "item/state/elementvalid.h"
#include "item/state/elementinvalid.h"

ElementChanged::ElementChanged()
    : ElementState()
{

}

bool ElementChanged::isSpaced() const {
    return false;
}

void ElementChanged::entry(AbstractElement *element) const {
    Item *item = dynamic_cast<Item*>(element);
    if (item) {
        item->setDefaultTextColor(Qt::gray);
    }

    handleChange(element);

}

void ElementChanged::exit(AbstractElement *element) const {
//    qDebug() << "changed exit: ";
//    Item *item = dynamic_cast<Item*>(element);
//    if (item) {
//        if (element->styleE()->getIsColor()) {
//            item->setDefaultTextColor(Qt::blue);
//        } else {
//            item->setDefaultTextColor(Qt::black);
//        }
//    }
}

void ElementChanged::edited(AbstractElement *element) const {
    handleChange(element);
}

void ElementChanged::handleChange(AbstractElement *element) const {

    element->notify();
}
