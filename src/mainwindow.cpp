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
#include <QAction>
#include <QTime>
#include <QShortcut>
#include "scene/blockscene.h"
#include "scene/parser.h"


MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createToolbars();



    view = new QGraphicsView();
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    scene = BlockScene::instance(view);
    view->setScene(scene);
    view->showMaximized();
    minimap = new MiniMapView();
    minimap->setScene(scene);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    //layout->addWidget(lineEditor);
    layout->addWidget(minimap);
    minimap->setFixedWidth(120);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle(tr("BlockEditor"));
    setUnifiedTitleAndToolBarOnMac(true);


    _textType = "lua";
    parser = Parser::instance(_textType);

//    QObject::connect( parser, &Parser::addElementItem,
//                     scene, &BlockScene::addParserItem );
//    QObject::connect( parser, &Parser::addElementLayout,
//                     scene, &BlockScene::addParserLayout );
//    QObject::connect( parser, &Parser::parsingFinished,
//                      scene, &BlockScene::updateTreeNeighbors);

    //setGeometry(100, 100, 500, 400);
    scene->setSceneRect(0, 0, 500, 480);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *a = fileMenu->addAction(tr("&New"), this, SLOT(newFile()), QKeySequence::New);
    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()), QKeySequence::Open);
    fileMenu->addAction(tr("&Save..."), this, SLOT(saveFile()), QKeySequence::Save);
    fileMenu->addAction(tr("&SaveAs..."), this, SLOT(saveFileAs()), QKeySequence::SaveAs);
    //fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), QKeySequence::Quit);

    //itemMenu = menuBar()->addMenu(tr("&Item"));
    //itemMenu->addAction(deleteAction);
    //itemMenu->addSeparator();

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
    //lineEditor->clear();
   // fileName =
    //TODO fix this
    _text = "";
    fileName = "";
    scene->addNewRoot(parser->parse(_text));

}

void MainWindow::openFile(const QString &path)
{

    fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Lua scripts (*.lua)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {

            //QFileInfo fileInfo(file);

            //_textType = fileInfo.suffix();

            QTextStream in(&file);
            _text = in.readAll();
            //lineEditor->setPlainText(_text);
            file.close();
            qDebug() << "--- start parsing time measure -------";
            QTime time;
            time.start();
            scene->addNewRoot(parser->parse(_text));
            double runTime = time.elapsed();// / 1000.0;
            qDebug() << "--- parsing time= "<< QString::number(runTime, 'f', 2);
        }
    }
}


void MainWindow::saveFile()
{
    if (fileName.isEmpty()){
      //  saveFileAs();
    }else {
        saveOperation();
    }
}

void MainWindow::saveOperation(){
    if (fileName.isEmpty()){
        return;
    }
    QFile f(fileName);
    f.open( QIODevice::WriteOnly );
    QTextStream out(&f);
    out <<  scene->root()->textE();
    f.close();
}


void MainWindow::saveFileAs()
{
    fileName= QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("LUA script (*.lua);;All Files (*)"));
    saveOperation();
}
