#include "item/abstractelement.h"
#include <QDebug>


#include <QGraphicsLinearLayout>
#include "item/layout.h"
#include "item/item.h"
#include "item/state/elementstate.h"
#include "style/style.h"
#include "scene/elementobserver.h"
#include "vector"



AbstractElement::AbstractElement(QGraphicsLinearLayout *layoutParrent)
{
    _layoutParrentor = static_cast<Layout*>(layoutParrent);
    _next = NULL;
    _previous = NULL;
    _enablePaint = false;
    _state = NULL;

}


// --------------------------------------
// -- get/set ---------------------------
// --------------------------------------

AbstractElement *AbstractElement::getNext() const {
    return _next;
}

AbstractElement *AbstractElement::getPrevius() const {
    return _previous;
}

QString AbstractElement::getType() const {
    return _type;
}

Layout* AbstractElement::getLayoutParrent() const {
    return _layoutParrentor;
}

Style *AbstractElement::styleE() const {
    return _style;
}

ElementState *AbstractElement::state() const {
    return _state;
}

bool AbstractElement::isPaintEnabled() const {
    return _enablePaint;
}

int  AbstractElement::getCurPos() const {
    return _curpos;
}


void AbstractElement::setNext(AbstractElement *next) {
    _next = next;
}

void AbstractElement::setPrevius(AbstractElement *previous) {
    _previous = previous;
}

void AbstractElement::setType(QString type) {
    _type = type;
}

void AbstractElement::setLayoutParrent(Layout *parrent) {
    _layoutParrentor = parrent;
}

void AbstractElement::setState(ElementState *state) {
    if (_state != NULL) {
        state->exit(this);
    }
    _state = state;
    _state->entry(this);
}

void AbstractElement::setStyleE(Style *style) {
    _style = style;
}

void AbstractElement::setPaintEnable( bool enablePaint ) {
    _enablePaint = enablePaint;
}

void AbstractElement::setCurPos(int curpos) {
    _curpos = curpos;
}

// --------------------------------------
// -- other methods ---------------------
// --------------------------------------

QString AbstractElement::toString() const {
    QString parentType= "";
    if(getLayoutParrent()) {
        parentType = getLayoutParrent()->getType();
    }
    return getType() + "   " + parentType + "  " + textE();
}

AbstractElement *AbstractElement::nextPrevius(bool next) const {
    if(next) {
        //qDebug() << "next";
        return getNext();
    } else {
        //qDebug() << "previous";
        return getPrevius();
    }
}

AbstractElement *AbstractElement::nextPreviousAlsoHor(bool toNext) {

    const AbstractElement *element = this;
    Layout *parent = element->getLayoutParrent();

    while (parent != NULL && parent->orientation() == Qt::Horizontal) {
        if (NULL != element->nextPrevius(toNext)) {
            return firstLastItemOf( element->nextPrevius(toNext), toNext);
        }
        element = parent;
        parent = element->getLayoutParrent();
    }
    return NULL;
}

AbstractElement *AbstractElement::nextPreviousAlsoVert(bool toNext) {
    const AbstractElement *element = this;
    do {
        if (NULL != element->nextPrevius(toNext)) {
            return firstLastItemOf( element->nextPrevius(toNext), toNext);
        }
        element = element->getLayoutParrent();
    } while (element != NULL);
    return NULL;
}

AbstractElement *AbstractElement::firstLastItemOf(AbstractElement *parent, bool first) {
    if (parent == NULL) {
        return NULL;
    }
    return parent->firstLastItem(first);
}

AbstractElement *AbstractElement::firstLastItem(bool first) {
    AbstractElement *targed = this;
    while (targed->isLayoutE()) {
        Layout *lay = dynamic_cast <Layout*>(targed);
        QGraphicsLayoutItem  *layout = lay->firstLastChildrenElement(first);
        targed = dynamic_cast <AbstractElement*>(layout);
    }
    return targed;
}

bool AbstractElement::isParent(AbstractElement *checkedParent) {
    AbstractElement *parent = dynamic_cast<AbstractElement*>(this->getLayoutParrent());
    while (parent) {
        if (parent == checkedParent) {
            return true;
        }
        parent = dynamic_cast<AbstractElement*>( parent->getLayoutParrent());
    }
    return false;
}

void AbstractElement::setCursorPosition(int pos) {
    AbstractElement *targed = this;
    while (targed->isLayoutE()) {
        Layout *layout = dynamic_cast<Layout*>(targed);
        AbstractElement *child = dynamic_cast<AbstractElement *>(layout->itemAt(0));

        int len = posibleAbsoluteSkip(child, pos);
        while ( 0 < len) {
            pos -= len;
            child = child->getNext();
            len = posibleAbsoluteSkip(child, pos);
        }
        targed = child;
    }

    int len = targed->textLength();
    pos = pos < len ? pos : len;
    Item *item = dynamic_cast <Item*>(targed);
    item->setFocus();
    QTextCursor cursor = item->textCursor();
    cursor.setPosition(pos);
    item->setTextCursor(cursor);
}

AbstractElement *AbstractElement::findElementOnPosition(int pos) {
    //qDebug() << "findElementOnPosition:"<< pos;
    AbstractElement *targed = this;
    while (targed->isLayoutE()) {
        //qDebug() << "findElementOnPosition:"<< pos;
        Layout *layout = dynamic_cast<Layout*>(targed);
        AbstractElement *child = dynamic_cast<AbstractElement *>(layout->itemAt(0));

        int len = posibleAbsoluteSkip(child, pos);
        while ( 0 < len) {
            //qDebug() << "skip:"<< pos;
            pos -= len;
            child = child->getNext();
            len = posibleAbsoluteSkip(child, pos);
        }
        targed = child;
    }
    return targed;
}

int AbstractElement::posibleAbsoluteSkip(AbstractElement *child, int pos) const {
    if (child == NULL) {
        return 0;
    }
    int len = child->textLength();
    if (len < pos
            && NULL != child->getNext()) {
        return len;
    }
    return 0;
}

AbstractElement *AbstractElement::findMutualParent(AbstractElement *second) {
    qDebug() << "find mutual";
    std::vector<AbstractElement*> fParents;
    std::vector<AbstractElement*> sParents;
    AbstractElement *p = this;
    while (p) {
        fParents.push_back(p);
        p = p->getLayoutParrent();
    }
    p = second;
    while (p) {
        sParents.push_back(p);
        p = p->getLayoutParrent();
    }
    int si = sParents.size()-1;
    int fi = fParents.size()-1;

    AbstractElement *parent = NULL;
    while (si >= 0 && fi >= 0
           && sParents.at(si) == fParents.at(fi)) {
        parent = sParents.at(si);
        si--;
        fi--;
    }
    if (NULL == parent) {
        qDebug() << "WARNING findMutualParent: now one same parent!";
    }
    fParents.clear();
    sParents.clear();
    return parent;
}

AbstractElement *AbstractElement::getParsableParent() {
    //qDebug() << "getParseble parent for: "<< getType() << "   " << textE();
    AbstractElement *element = this;
    while ( element->getLayoutParrent()
            && !(element->styleE()->getIsParsable())) {
        element = element->getLayoutParrent();
        //qDebug() << " * new PP: "<< element->getType() << "   " << element->textE();
    }
    return element;
}

AbstractElement *AbstractElement::getSecondParsable() {
    AbstractElement *element = getParsableParent();
    if (element->getLayoutParrent()) {
        element = element->getLayoutParrent()->getParsableParent();
    }
    return element;
}


void AbstractElement::edited() {
    state()->edited(this);
}

// -- element observers ----------------------------

void AbstractElement::attach(ElementObserver *observer) {
    std::list<ElementObserver*>::const_iterator it;
    for (it = _observers.cbegin(); it != _observers.cend(); it++) {
        if  ((*it) == observer) {
            return;
        }
    }
    _observers.push_back(observer);
}

void AbstractElement::detach(ElementObserver *observer) {
    std::list<ElementObserver*>::iterator it;
    for (it = _observers.begin(); it != _observers.end(); it++) {
        if  ((*it) == observer) {
            _observers.erase(it);
            return;
        }
    }
}

void AbstractElement::notify() {
    //qDebug() << "NOTIFY for "<< textE();
    std::list<ElementObserver*>::iterator it, itprev;
    it = _observers.begin();
    while (it != _observers.end()) {
        itprev = it;
        it++;
        //qDebug() << "notify update " << textE();
        (*itprev)->update(this);
    }
}
