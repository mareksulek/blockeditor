#ifndef BASICITEM_H
#define BASICITEM_H

#include <QGraphicsTextItem>
#include <QSizeF>
#include <list>

class QGraphicsLinearLayout;
class Style;
class Layout;
class ElementState;
class ElementObserver;
class Item;

class AbstractElement {

public:
    explicit AbstractElement(QGraphicsLinearLayout *layoutParrent);

    // get/set
    AbstractElement *getNext() const;
    AbstractElement *getPrevius() const;
    QString getType() const;
    Layout  *getLayoutParrent() const;
    Style   *styleE() const;
    ElementState *state() const;
    bool    isPaintEnabled() const;
    int  getCurPos() const;

    void setNext(AbstractElement *next);
    void setPrevius(AbstractElement *previous);
    void setType(QString type);
    void setLayoutParrent(Layout *parrent);
    void setState(ElementState *state);
    void setCurPos(int curpos);


    // other methods
    QString toString() const;
    AbstractElement *nextPrevius(bool next) const;
    AbstractElement *nextPreviousAlsoHor(bool toNext);
    AbstractElement *nextPreviousAlsoVert(bool toNext);
    AbstractElement *firstLastItem(bool first);
    AbstractElement *firstLastItemOf(AbstractElement *parent, bool first);
    bool isParent(AbstractElement *checkedParent);
    void setCursorPosition(int pos);
    AbstractElement *findElementOnPosition(int pos);
    int posibleAbsoluteSkip(AbstractElement *child, int pos) const;
    AbstractElement *findMutualParent(AbstractElement *second);
    AbstractElement *getParsableParent();
    AbstractElement *getSecondParsable();

    void edited();
    virtual void setStyleE(Style *style);
    virtual void setPaintEnable( bool enablePaint );
    // element observer
    virtual void attach(ElementObserver *observer);
    virtual void detach(ElementObserver *observer);
    virtual void notify();

    // pure virtual methods
    virtual QSizeF elementSizeHint(Qt::SizeHint which) const = 0;
    virtual bool isLayoutE() const = 0;
    virtual int textLength(bool length = true) const = 0;
    virtual QString textE() const = 0;
    virtual QPixmap toPixmap() = 0;


protected:
    QString _type;
    bool _elementType;
    bool _enablePaint;   // TODO
    int  _curpos;

    AbstractElement *_next;
    AbstractElement *_previous;

    Layout  *_layoutParrentor;  // TODO
    Style   *_style;
    ElementState *_state;
    std::list<ElementObserver*> _observers;
};

#endif
