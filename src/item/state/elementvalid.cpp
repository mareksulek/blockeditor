#include "item/state/elementvalid.h"
#include <QDebug>

#include "item/abstractelement.h"
#include "item/state/elementchanged.h"
#include "item/abstractelement.h"

ElementValid::ElementValid()
    : ElementState()
{

}


bool ElementValid::isSpaced() const {
    return false;
}


void ElementValid::entry(AbstractElement *element) const {
    element->setStyleE(element->styleE());

}

void ElementValid::exit(AbstractElement *element) const {

}

void ElementValid::edited(AbstractElement *element) const {
    element->setState( new ElementChanged);
}
