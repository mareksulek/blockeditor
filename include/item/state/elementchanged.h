#ifndef ELEMENTCHANGED_H
#define ELEMENTCHANGED_H

#include "item/state/elementstate.h"

class AbstractElement;

class ElementChanged : public ElementState
{

public:
    explicit ElementChanged();

    void entry(AbstractElement *element) const;
    void exit(AbstractElement *element) const;
    void edited(AbstractElement *element) const;
    bool isSpaced() const;
    bool hasInvalidText() const {return false;}

    void handleChange(AbstractElement *element) const;

};

#endif
