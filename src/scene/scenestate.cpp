#include "scene/scenestate.h"
#include "QDebug"

#include "item/abstractelement.h"
#include "item/item.h"

SceneState::SceneState()
{
    _selectedItem = NULL;
    _paintedElement = NULL;
}



Item *SceneState::getSelectedItem() const {
    return _selectedItem;
}

AbstractElement *SceneState::getPaintedElement() const {
    return _paintedElement;
}

void SceneState::setSelectedItem(Item *selectedItem) {
    _selectedItem = selectedItem;
}

void SceneState::setPaintedElement(AbstractElement *paintedElement) {
    _paintedElement = paintedElement;
}



