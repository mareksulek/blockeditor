#include "item/state/elementstable.h"
#include <QDebug>


ElementStable::ElementStable()
    : ElementState()
{

}

bool ElementStable::isSpaced() const {
    return true;
}


void ElementStable::entry(AbstractElement *) const {

}

void ElementStable::exit(AbstractElement *) const {

}

void ElementStable::edited(AbstractElement *element) const {

}
