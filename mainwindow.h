#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsSceneEvent>
#include "card.h"
#include "gamecontroller.h"
#include <QMenu>
#include <QMenuBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initScene();
    void initSceneBackground();

    QGraphicsScene *scene;
    QGraphicsView *view;
    GameController *game;

};

#endif // MAINWINDOW_H
