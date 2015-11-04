#include "mainwindow.h"

#include <QTextEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>

#include "mainview.h"
#include "minimapview.h"
#include "lineeditor.h"
#include "scene/blockscene.h"
#include "scene/linenumbers.h"
#include "highlighter.h"
#include "scene/parser.h"


MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createToolbars();

    lineEditor = new LineEditor(/*this*/);
    highlighter = new Highlighter(lineEditor->document());


    //view = new QGraphicsView();
    view = new MainView();
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    scene = BlockScene::instance(view);
    view->setScene(scene);
    view->showMaximized();
    minimap = new MiniMapView();
    minimap->setScene(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    layout->addWidget(minimap);
    minimap->setFixedWidth(100);
    //layout->addWidget(lineEditor);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle(tr("BlockEditor"));
    setUnifiedTitleAndToolBarOnMac(true);

    _textType = "lua";
    parser = new Parser(_textType);

    QObject::connect( parser, &Parser::addElementItem,
                     scene, &BlockScene::addParserItem );
    QObject::connect( parser, &Parser::addElementLayout,
                     scene, &BlockScene::addParserLayout );
    QObject::connect( parser, &Parser::parsingFinished,
                      scene, &BlockScene::updateTreeNeighbors);

    setGeometry(100, 100, 500, 400);
    //scene->setSceneRect(0, 0, 470, 380);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&New"), this, SLOT(newFile()), QKeySequence::New);
    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()), QKeySequence::Open);
    //fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), QKeySequence::Quit);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    //itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(tr("&About"), this, SLOT(about()));
    //aboutMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
}

void MainWindow::createActions() { }

void MainWindow::createToolbars() { }


void MainWindow::about()
{
    QMessageBox::about(this, tr("About Block Editor"),
                       tr("<p>The <b>Block Editor</b> is prototype of hybrid text and visual editor for Lua script language, that use LPEG for syntax analysm of edited code.</p>"));
}

void MainWindow::newFile()
{
    lineEditor->clear();
}

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Lua scripts (*.lua)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {

            //QFileInfo fileInfo(file);

            //_textType = fileInfo.suffix();

            QTextStream in(&file);
            _text = in.readAll();
            lineEditor->setPlainText(_text);
            file.close();
            parser->parse(_text);
        }
    }
}
