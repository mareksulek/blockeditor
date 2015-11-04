#include "style/style.h"

#include <QDebug>
#include <QStringBuilder>

Style::Style( const QString styleType)
{
    _styleType = styleType;
    _orientation = OrientationEnum::any;
    _isItem = false;
    _isLayout = false;
    _isColor = false;
    _isContext = false;
    _isTransparent = false;
    _background = QColor(0,0,0,0);
}


QString Style::toString() const
{
    return "Style: " + getStyleType() \
            + (getIsItem() ? "\t\tisItem":"\t") \
            + (getIsLayout() ? "\tisLayout":"\t") \
            + (getIsLayout() ?(getOrientation()==OrientationEnum::vertical \
                           ?"\tvertical":"\thorizontal"):"\t") \
            + (getIsColor() ? "\tisColor":"\t");
}

QString Style::getStyleType() const {
    return _styleType;
}

OrientationEnum Style::getOrientation() const {
    return _orientation;
}

bool Style::getIsItem() const {
    return _isItem;
}

bool Style::getIsLayout() const {
    return _isLayout;
}

bool Style::getIsColor() const {
    return _isColor;
}

bool Style::getIsTransparent() const {
    return _isTransparent;
}

bool Style::getIsContext() const {
    return _isContext;
}

QColor Style::getBackground() const {
    return _background;
}

void Style::setOrientation(OrientationEnum orientation) {
    _orientation = orientation;
}
void Style::setOrientation(QString orientation) {
    if (0 == orientation.compare("horizontal")) {
        setOrientation(OrientationEnum::horizontal);
    }
    else if (0 == orientation.compare("vertical")) {
        setOrientation(OrientationEnum::vertical);
    }
}

void Style::setIsItem(bool isItem) {
    _isItem = isItem;
}

void Style::setIsLayout(bool isLayout) {
    _isLayout = isLayout;
}

void Style::setIsColor(bool isColor) {
    _isColor = isColor;
}

void Style::setIsTransparent(bool isTransparent) {
    _isTransparent = isTransparent;
}

void Style::setIsContext(bool isContext) {
    _isContext = isContext;
}

void Style::setBackground(QColor color) {
    _background = color;
}

void Style::setBackground(float r, float g, float b, float a) {
    _background = QColor(r*255, g*255, b*255, a*255);
}
