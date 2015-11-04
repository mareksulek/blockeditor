#ifndef ELEMENTBUILDER_H
#define ELEMENTBUILDER_H


class AbstractElement;
class ElementState;
class Layout;

class ElementBuilder
{

public:

    virtual void buildItem(Layout *parent, QString text) = 0;
    virtual void buildLayout(Layout *parent) = 0;
    virtual void buildStyle() = 0;
    virtual void buildState() = 0;
    virtual void buildState(ElementState *state) = 0;
    virtual void buildType(QString type) = 0;

    virtual AbstractElement* getElement() = 0;

};


#endif
