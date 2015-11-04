#include "style/styleutil.h"

#include <QDebug>
#include <QGraphicsLinearLayout>
#include "style/style.h"


StyleUtil* StyleUtil::_instance;

StyleUtil* StyleUtil::instance( )
{
    if (NULL == _instance){
        _instance = new StyleUtil();
    }
    return _instance;
}

// private constructor
StyleUtil::StyleUtil()
{
}

/**
 * @brief StyleUtil::getStyle return pointer to style by type
 * @param type
 * @return pointer to style
 */
Style* StyleUtil::getStyle(QString type) const
{
    Style *style = _styleMap.value(type);
    // TODO
    return style;
}


void StyleUtil::addStyle( Style *style) {
    qDebug() << "Inserted" << style->toString();
    _styleMap.insert(style->getStyleType(), style);
}
