#ifndef ELEMENTSTATE_H
#define ELEMENTSTATE_H

class AbstractElement;

class ElementState
{

public:
    explicit ElementState();

    virtual void entry(AbstractElement *element) const = 0;
    virtual void exit(AbstractElement *element) const = 0;
    virtual void edited(AbstractElement *element) const = 0;
    virtual bool isSpaced() const = 0;
    virtual bool hasInvalidText() const = 0;

};

#endif
