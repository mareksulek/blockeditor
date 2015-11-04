#ifndef REPARSER_H
#define REPARSER_H

#include "scene/elementobserver.h"

class AbstractElement;
class Layout;

class Reparser : public ElementObserver{
public:
    explicit Reparser();
    ~Reparser();

    void update(AbstractElement *element);

    Layout *findBiggerParnet(AbstractElement *element);
    AbstractElement *firstChild(Layout *parent);
    AbstractElement *firstChild(AbstractElement *parent);
    void replaceElement(AbstractElement *oldElem, AbstractElement*newElem);

    void reparse(AbstractElement *element);


private:


};

#endif
