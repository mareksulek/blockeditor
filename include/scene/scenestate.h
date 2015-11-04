#ifndef SCENESTATE_H
#define SCENESTATE_H


class AbstractElement;
class Item;

class SceneState
{
public:
    SceneState();

    Item *getSelectedItem() const;
    AbstractElement *getPaintedElement() const;
    void setSelectedItem(Item *selectedItem);
    void setPaintedElement(AbstractElement *paintedElement);

private:
    Item *_selectedItem;
    AbstractElement *_paintedElement;

};

#endif // MAINWINDOW_H
