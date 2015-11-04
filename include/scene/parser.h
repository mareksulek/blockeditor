#ifndef PARSER_H
#define PARSER_H


#include <LuaState.h>
#include <QString>

#include "style/styleutil.h"


class Item;
class Layout;
class Style;
class AbstractElement;
class ElementBuilder;

class Parser {


public:
    static Parser *instance(QString type = "");

    Layout *parse(QString text);
    bool reparse(QString text, AbstractElement **res, int &parsedChars);

    Item *createNewItem(Layout *parent, QString type, QString text);
    Item *createStableItem(Layout *parent, QString text);
    //Item *createChangedItem(Layout *parent, QString text);
    Layout *createNewLayout(Layout *parent, QString type);

private:
    static Parser *_instance;
    Parser(QString type);


    void init();
    void loadGrammar();
    void loadStyle();

    lua::State _state;
    QString _textType;
    QString _text;
    ElementBuilder *_elementBuilder;

    Layout *_retree;
    int _parsedChars;

};

#endif // MAINWINDOW_H
