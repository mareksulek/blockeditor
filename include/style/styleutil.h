#ifndef STYLEMAP_H
#define STYLEMAP_H

#include <QString>
#include <QMap>
class Style;


class StyleUtil
{
public:
    static StyleUtil* instance( );

    Style* getStyle(QString type) const;
    void addStyle(Style *style);

private:
    StyleUtil();

    static StyleUtil* _instance;
    QMap<QString, Style*> _styleMap;
};

#endif
