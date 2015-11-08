#include "mainwindow.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow window;

    window.show();
    window.openFile("examples_scripts/echo.lua");
    //window.openFile("examples_scripts/big.lua");


    //window.openFile("scripts/core.lua");
     //window.openFile("scripts/parser.lua");
     //window.openFile("scripts/grammars/lua/grammar.lua");
     //window.openFile("scripts/grammars/lua/style_new.lua");


    //window.openFile("scripts/libs/lpeglj/init.lua");
     //window.openFile("scripts/libs/lpeglj/lpvm.lua");
     //window.openFile("scripts/libs/lpeglj/lpcode.lua");

    return app.exec();
}
