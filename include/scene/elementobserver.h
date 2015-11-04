#ifndef ELEMENTOBSERVER_H
#define ELEMENTOBSERVER_H

class AbstractElement;

class ElementObserver {
public:
    explicit ElementObserver();

    virtual void update(AbstractElement *element) = 0;

};

#endif
