#include "scene/reparser.h"
#include <QDebug>

#include "item/abstractelement.h"
#include "item/item.h"
#include "item/layout.h"
#include "scene/parser.h"
//#include "item/state/elementstate.h"
#include "item/state/elementinvalid.h"
//#include "item/state/elementvalid.h"

#include "scene/blockscene.h"
#include "item/abstractelement.h"
#include "item/layout.h"
#include "item/item.h"
#include <QTextCursor>
#include <QTime>


Reparser::Reparser()
    : ElementObserver() {
}

Reparser::~Reparser() {
    qDebug() << "destructor";
}

void Reparser::update(AbstractElement *element) {
    qDebug() << " REPARSER update: "<< element->textE();
    reparse(element);
    qDebug() << " end REPARSER";
}

void Reparser::reparse(AbstractElement *element) {
    qDebug() << "--- start parsing time measure -------";
    QTime time;
    time.start();
    int parsedChars = 0;
    AbstractElement *res = NULL;
    QString text = element->textE();
    qDebug()<< "text len = " << text.length();

    bool ok = Parser::instance()->reparse(text, &res, parsedChars);

    if (ok) {
        qDebug() << "Reparsing OK";
        qDebug() << "parsed c= "<< parsedChars;

        replaceElement(element, res);
        res->setCursorPosition( element->getCurPos());
        //element->setState( new ElementValid);
    } else {
        qDebug() << "Reparsing FAIL";
        AbstractElement *okElement = element->findElementOnPosition(parsedChars-1);
        qDebug() << "Element on pos ="<< okElement->textE();
        AbstractElement *wrongElement = element;

        if (parsedChars > 1) {
            // possible NULL pointer  TODO !!!
            //            while (NULL == okElement->getNext()) {
            //                okElement = okElement->getLayoutParrent();
            //            }
            //            wrongElement = okElement->getNext();
            wrongElement = okElement->getLayoutParrent();

        }
        //qDebug() << "WRONG element="<< wrongElement->textE();
        wrongElement->setState( new ElementInvalid);

    }



    double runTime = time.elapsed();// / 1000.0;
    qDebug() << "--- parsing time= "<< QString::number(runTime, 'f', 2);
    qDebug()<< "------ reparsed text len = " << text.length();

}

void Reparser::replaceElement(AbstractElement *oldElem, AbstractElement*newElem) {
    Layout *parent = oldElem->getLayoutParrent();
    if (NULL == parent) {
        // TODO ????? maybe new ROOT
        qDebug() << "WARNING !!! reparser: replace element with NULL parent!";

        Layout *newRoot = dynamic_cast<Layout *>(newElem);
        Layout *oldRoot = dynamic_cast<Layout *>(oldElem);
        if (oldRoot && newRoot) {
            BlockScene::instance()->removeItem( dynamic_cast<QGraphicsItem *>(oldRoot->itemAt(0)), true);
            oldRoot->removeAt(0);

            QGraphicsLayoutItem *glItem = dynamic_cast<QGraphicsLayoutItem *>(newRoot->itemAt(0));
            AbstractElement *element = dynamic_cast<AbstractElement *>(glItem);

            oldRoot->addItem(glItem);
            BlockScene::instance()->addItem( dynamic_cast<QGraphicsItem *>(glItem), true);
            oldRoot->updateChildNeighbors();
            element->setLayoutParrent(oldRoot);
        } else {
            qDebug() << "WARNING !!! reparser not new/old roots";
        }

    } else {
        newElem = firstChild(newElem);
        qDebug() << "res = "<< newElem->getType();

        parent->insertBehind(oldElem, newElem);
        BlockScene::instance()->addItem( dynamic_cast<QGraphicsItem*>(newElem), true);

        parent->removeElement(oldElem);
        BlockScene::instance()->removeItem( dynamic_cast<QGraphicsItem *>(oldElem), true);
        parent->updateChildNeighbors();
        qDebug() << "replaced";
    }


}


Layout *Reparser::findBiggerParnet(AbstractElement *element) {
    Layout *parent = element->getLayoutParrent();
    int elementTextLen = element->textE().length();
    while (parent != NULL && parent->textE().length() == elementTextLen) {
        parent = parent->getLayoutParrent();
    }
    return parent;
}


AbstractElement *Reparser::firstChild(AbstractElement *parent) {
    Layout *layout = dynamic_cast<Layout*>(parent);
    if (layout) {
        return firstChild(layout);
    }
    return NULL;
}

AbstractElement *Reparser::firstChild(Layout *parent) {
    if (parent->count() != 1) {
        qDebug () << "WARNING!!! reparser:firstChild,  no exactly 1 child!";
        return NULL;
    }
    return dynamic_cast<AbstractElement*>(parent->itemAt(0));
}


