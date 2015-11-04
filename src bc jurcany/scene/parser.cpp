#include "scene/parser.h"
#include "item/item.h"
#include "item/textitem.h"
#include "item/blankitem.h"
#include "item/layout.h"
#include "style/style.h"
#include <QDebug>
#include <QStringBuilder>

Parser::Parser(QString type) :
    QObject()
{
    _textType = type;
    init();
    loadGrammar();
    loadStyle();
}



void Parser::init() {
      //_state.set("send_commit");
    _state.set("addBasicItem",
               [this] (lua::String elementType,
                       lua::String elementText,
                       lua::Pointer parentPointer,
                       lua::Integer elementIndex)
               -> lua::Pointer
    {
        //elementText.
        QString text = elementText;
        QString onlyText;
        QString afterText;

        QString::ConstIterator it;
        for (it = text.constBegin(); it != text.constEnd(); it++) {
            //qDebug() << "--------- " << *it << "  space:"<<it->isSpace() <<"  isLN:"<<it->isLetterOrNumber() <<"  isPrint:"<<it->isPrint() <<"  isSymbol:"<<it->isSymbol();
            if (it->isSpace()) {
                //afterText += *it;
            } else {
                onlyText += *it;
            }
        }
        qDebug()<<" >"<< onlyText <<"<>"<<afterText;


        Item *newItem= new TextItem( elementType, onlyText, StyleUtil::instance()->getStyle(elementType), static_cast<Layout*>(parentPointer));
        emit addElementItem(newItem);

        if (!afterText.isEmpty()){
            Item *newItem= new BlankItem( elementType, afterText, StyleUtil::instance()->getStyle(elementType), static_cast<Layout*>(parentPointer));
            emit addElementItem(newItem);
        }

        return newItem;
    });
    _state.set("addBasicLayout",
               [this] (lua::String elementType,
                       lua::Pointer parentPointer,
                       lua::Integer elementIndex)
               -> lua::Pointer
    {
        Layout *newLayout= new Layout( elementType, StyleUtil::instance()->getStyle(elementType), static_cast<Layout*>(parentPointer));
        emit addElementLayout(newLayout);

        return newLayout;
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
        lua::String object = style["object"];
        newStyle->setIsContext( style["context"]);

        if (strcmp(object, "text") == 0){
            lua::Value text = style["text"];
            newStyle->setIsColor( text["isColor"]);
        } else {
            newStyle->setOrientation( object);
        }

        if (style["background"]){
            lua::Value c = style["background"]["color"];
            newStyle->setBackground(c[1].toFloat(), c[2].toFloat(), c[3].toFloat(), c[4].toFloat());
        }
        if (style["transparent"]){
            newStyle->setBackground(Qt::transparent);
            newStyle->setIsTransparent(true);
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

void Parser::parse(QString text) {
    qDebug() << "Request text reparse...";
    _state["parseTextNew"]( text.toStdString().c_str());
    qDebug() << "... parsing DONE";
    emit parsingFinished();
}
