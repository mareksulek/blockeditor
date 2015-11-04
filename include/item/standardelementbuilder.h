#ifndef STANDARDELEMENTBUILDER_H
#define STANDARDELEMENTBUILDER_H

#include "item/elementbuilder.h"

class QGraphicsLinearLayout;
class AbstractElement;
class Style;


class StandardElementBuilder :   public ElementBuilder
{

public:
    explicit StandardElementBuilder();



    void buildItem(Layout *parent, QString text);
    void buildLayout(Layout *parent);
    void buildStyle();
    void buildState();
    void buildState(ElementState *state);
    void buildType(QString type);

    AbstractElement* getElement();

private:
    QString _elementType;
    AbstractElement *_element;

};


#endif
