#ifndef BLOCKSCENE_H
#define BLOCKSCENE_H

#include <QGraphicsScene>

#include "item/item.h"
#include "item/layout.h"
#include "QStack"



class QGraphicsLinearLayout;
class QGraphicsWidget;
class SceneEventObserver;
class SceneState;
class Command;
class Reparser;

class BlockScene : public QGraphicsScene
{
public:
    static BlockScene *instance( QObject *parent = 0);
    void addItem(QGraphicsItem *graphicItem, bool recursive = false);
    void removeItem(QGraphicsItem *graphicItem, bool recursive = false);


    SceneState *getSceneState() const { return _sceneState;}
    void addCommand(Command *command);
    void addNewRoot(Layout *root);

    Layout *root() const;
    void  setRoot(Layout *root);


private:
    static BlockScene *inst;

    BlockScene( QObject *parent = 0);

    QGraphicsLinearLayout *_vLayout;
    QGraphicsWidget *_form;
    Layout *_root;
    SceneEventObserver *_eventFilter;
    Reparser *_reparser;
    SceneState *_sceneState;

    QStack<Command *> _commandStack;

};

#endif // MAINWINDOW_H
