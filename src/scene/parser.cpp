#include "scene/parser.h"
#include "item/item.h"
#include "item/layout.h"
#include "item/abstractelement.h"
#include "style/style.h"
#include <QDebug>
#include <QStringBuilder>
#include <item/state/elementvalid.h>
#include <item/state/elementstable.h>
#include <item/state/elementchanged.h>
#include <item/elementbuilder.h>
#include <item/standardelementbuilder.h>
#include <QGraphicsLinearLayout>


Parser *Parser::_instance = NULL;

Parser *Parser::instance(QString type) {
    if (_instance == NULL) {
        if (type.isEmpty()) {
            qDebug() << "Warning: parser type undefined!";
        }
        _instance = new Parser(type);
    }
    return _instance;
}

Parser::Parser(QString type)
{
    _textType = type;
    _elementBuilder = new StandardElementBuilder();
    init();
    loadGrammar();
    loadStyle();
}



void Parser::init() {

    _state.set("createBasicLayout",
               [this] (lua::String elementType,
               lua::Pointer parentPointer,
               lua::Integer elementIndex)
            -> lua::Pointer
    {

        Layout *newLayout= createNewLayout( static_cast<Layout*>(parentPointer), elementType);
        if( newLayout->parentLayoutItem()){
            newLayout->getLayoutParrent()->addItem(newLayout);
        } else {
            //qDebug() << "ROOOOOOOOOOOOT";
            _retree = newLayout;
        }
        //qDebug() << "parsing layout "<< elementType << "  " << dynamic_cast<AbstractElement*>(newLayout);
        return newLayout;
    });

    _state.set("createBasicItem",
               [this] (lua::String elementType,
               lua::String elementText,
               lua::Pointer parentPointer,
               lua::Integer elementIndex)
            -> lua::Pointer
    {
        QString text = elementText;
        QString type = elementType;
        QString onlyText;
        QString afterText = "";
        QStringList emptywords;

        QString::ConstIterator it;
        bool wasEnter = false;
        bool stillComment = 0==type.compare("comment");

        for (it = text.constBegin(); it != text.constEnd(); it++) {
            if (it->unicode() == 10 || it->unicode() == 8233){
                stillComment = false;
                wasEnter = true;
                emptywords.push_back(afterText);
                afterText = "";
            } else {
                wasEnter = false;
                if (it->isSpace() && !stillComment) {
                    afterText += *it;
                } else {
                    onlyText += *it;
                }
            }
        }
        if (!wasEnter){
            emptywords.push_back(afterText);

            int size = emptywords.size();
            if (size > 1) {
                emptywords[size-2] = emptywords[size-2]+emptywords[size-1];
                emptywords.pop_back();
            }
        }

        Item *newItem = NULL;
        Item *stableItem = NULL;
        Layout *parent = static_cast<Layout*>(parentPointer);

        if (emptywords.isEmpty() || emptywords.at(0).isEmpty()) {
            newItem = createNewItem( parent, elementType, onlyText);
            parent->addItem(newItem);

        } else {
            if (parent->orientation() != Qt::Horizontal) {
                Layout *horLayout = createNewLayout( parent, "aux_line");
                newItem = createNewItem( horLayout, elementType, onlyText);
                horLayout->addItem(newItem);
                stableItem = createStableItem( horLayout, emptywords.at(0));
                horLayout->addItem(stableItem);
                parent->addItem(horLayout);
            } else {
                newItem = createNewItem( parent, elementType, onlyText);
                parent->addItem(newItem);
                stableItem = createStableItem( parent, emptywords.at(0));
                parent->addItem(stableItem);
            }
        }

        if (emptywords.size() > 1) {
            while (parent->orientation() != Qt::Vertical) {
                parent = parent->getLayoutParrent();
            }
            for (int i=1; i<emptywords.size(); i++) {
                QString spaceText = emptywords.at(i).isEmpty()?" ":emptywords.at(i);
                stableItem = createStableItem( parent, spaceText);
                parent->addItem(stableItem);
            }
        }

        return newItem;
    });




    _state.set("sendRetree",
               [this] ( lua::Pointer pointer,
               lua::Boolean ok,
               lua::Integer parsedChars)
            -> lua::Pointer
    {
        qDebug() << "////////////////////////////////////";
        qDebug() << "sendRetree";
        //_retree = static_cast<AbstractElement*>(pointer);
        if (!_retree) {
            qDebug() << "WARNING!!! sendRetree - _retree after conversion == NULL";
            qDebug() << "WARNING!!! sendRetree - _retree after conversion == NULL" << parsedChars;
            _parsedChars = parsedChars;
        } else {
            _parsedChars = parsedChars;
            qDebug() << "sendRetree " << bool(ok) << "  " << _parsedChars;
        }

        return NULL;
    });

    _state.doFile("scripts/init.lua");
    _state.doFile("scripts/core.lua");

}



void Parser::loadStyle()
{
    _state.set("addStyle",
               [this] ( lua::Value style,
               lua::String elementType)
            -> lua::Pointer
    {
        Style *newStyle = new Style(elementType);

        newStyle->setIsItem( style["item"]);
        newStyle->setIsLayout( style["grid"]);
        newStyle->setIsParsable( style["parsable"]);
        lua::String object = style["object"];

        if (strcmp(object, "text") == 0){
            lua::Value text = style["text"];
            newStyle->setIsColor( text["isColor"]);
        } else {
            newStyle->setOrientation( object);
        }

        StyleUtil::instance()->addStyle(newStyle);

        return newStyle;
    });

    qDebug() << "Load styles...";
    _state["loadStyles"]();

}

void Parser::loadGrammar()
{
    try {
        _state["loadGrammarAndStyle"].call( _textType.toStdString().c_str());
    }
    catch (lua::RuntimeError ex) {
        //QMessageBox msgBox;
        //msgBox.setText(QString("Error while loading grammar style: ")
        //.append(_textType));
        // msgBox.setInformativeText(ex.what());
        //msgBox.setStandardButtons(QMessageBox::Ok);
        //msgBox.exec();
        qDebug() << "Error while loading grammar style: " << _textType;
        qDebug() << ex.what();
        throw std::runtime_error(ex.what());
    }
}

Layout *Parser::parse(QString text) {
    _retree = NULL;
    qDebug() << "Request text reparse...";
    _state["parseTextNew"]( text.toStdString().c_str());
    qDebug() << "... parsing DONE";
    return _retree;
}

bool Parser::reparse(QString text, AbstractElement **res, int &parsedChars) {
    *res = _retree = NULL;
    parsedChars = _parsedChars = 0;

    bool ok = true;

    try {

        //qDebug() << "Request text reparse for:\n"<< text;
        ok = _state["reparseTextNew"]( text.toStdString().c_str());
        //emit parsingFinished()
    } catch (lua::RuntimeError ex) {
        ok = false;
    }

    qDebug() << "... parsing DONE";
    parsedChars = _parsedChars;
    if (ok) {
        *res = _retree;
    }

    return ok;
}


Item *Parser::createNewItem(Layout *parent, QString type, QString text) {
    _elementBuilder->buildItem(parent, text);
    _elementBuilder->buildType(type);
    _elementBuilder->buildStyle();
    _elementBuilder->buildState();
    return dynamic_cast<Item*>(_elementBuilder->getElement());
}

Item *Parser::createStableItem(Layout *parent, QString text) {
    _elementBuilder->buildItem(parent, text);
    _elementBuilder->buildType("spaced_text");
    _elementBuilder->buildStyle();
    _elementBuilder->buildState( new ElementStable());
    return dynamic_cast<Item*>(_elementBuilder->getElement());
}

//Item *Parser::createChangedItem(Layout *parent, QString text) {
//    _elementBuilder->buildItem(parent, text);
//    _elementBuilder->buildType("changed_text");
//    _elementBuilder->buildStyle();
//    _elementBuilder->buildState( new ElementChanged);
//    return dynamic_cast<Item*>(_elementBuilder->getElement());
//}

Layout *Parser::createNewLayout(Layout *parent, QString type) {
    //qDebug() << "inside building layout , parent = "<< parent;
    _elementBuilder->buildLayout(parent);
    _elementBuilder->buildType(type);
    _elementBuilder->buildStyle();
    _elementBuilder->buildState();
    return dynamic_cast<Layout*>(_elementBuilder->getElement());
}
