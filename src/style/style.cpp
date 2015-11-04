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
    _isParsable = false;
}


QString Style::toString() const
{
    return "Style: " + getStyleType() \
            + (getIsItem() ? "\t\tisItem":"\t") \
            + (getIsLayout() ? "\tisLayout":"\t") \
            + (getIsLayout() ?(getOrientation()==OrientationEnum::vertical \
                           ?"\tvertical":"\thorizontal"):"\t") \
            + (getIsColor() ? "\tisColor":"\t") \
            + (getIsParsable() ? "\tisParsable":"\t");
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

bool Style::getIsParsable() const {
    return _isParsable;
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

void Style::setIsParsable(bool isParsable) {
    _isParsable = isParsable;
}
