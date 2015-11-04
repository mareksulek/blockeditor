#ifndef STYLE_H
#define STYLE_H

#include <QString>

enum OrientationEnum {vertical, horizontal, any} ;

class Style
{

public:
    Style( const QString _styleType);

    QString getStyleType() const;
    OrientationEnum getOrientation() const;
    bool getIsItem() const;
    bool getIsLayout() const;
    bool getIsColor() const;
    bool getIsParsable() const;

    void setOrientation(OrientationEnum orientation);
    void setOrientation(QString orientation);
    void setIsItem(bool isItem);
    void setIsLayout(bool isLayout);
    void setIsColor(bool isColor);
    void setIsParsable(bool isParsable);

    QString toString() const;

private:
    QString _styleType;
    OrientationEnum _orientation;
    bool _isItem;
    bool _isLayout;
    bool _isColor;
    bool _isParsable;

};

#endif
