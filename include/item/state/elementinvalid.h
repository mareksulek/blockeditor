#ifndef ELEMENTINVALID_H
#define ELEMENTINVALID_H

#include "item/state/elementstate.h"

class AbstractElement;

class ElementInvalid : public ElementState
{

public:
    explicit ElementInvalid();

    void entry(AbstractElement *element) const;
    void exit(AbstractElement *element) const;
    void edited(AbstractElement *element) const;
    bool isSpaced() const;
    bool hasInvalidText() const {return true;}

};

#endif
