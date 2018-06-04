//解决MSVC编译器中文乱码
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include <QString>
#include <QDebug>
#include "card.h"
#include "card.cpp"
#include "constant.h"
#include <QPainter>
#include "cardtemp.h"
#include "cardtemp.cpp"
#include "gamecontroller.h"
#include "gamecontroller.cpp"
#include "gameresultdlg.cpp"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QTextCodec>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      scene(new QGraphicsScene(this)),
      view(new QGraphicsView(scene,this)),
      game(new GameController(*scene,this))
{

    setWindowIcon(QIcon(":/icon/images/icon.ico"));
    setWindowTitle("空当接龙FreeCell        Author:SeanLiao");
    setCentralWidget(view);
    resize(WINDOW_WIDTH,WINDOW_HEIGHT);
    this->setFixedSize(this->width(),this->height());
    initScene();

    //菜单栏
    QMenuBar *mBar = menuBar();
    //添加菜单
    QMenu *pFile = mBar->addMenu("选项");
    //添加菜单项，添加动作
    QAction *pNew = pFile->addAction("开始游戏");
    QAction *pEnd = pFile->addAction("结束游戏");
    QAction *pInfo = pFile->addAction("关于");

    connect(pNew,&QAction::triggered,
            [=]()
            {
                game->startNewGame();
            });

    connect(pEnd,&QAction::triggered,
            [=]()
            {
                game->GameOver();
            });
    connect(pInfo,&QAction::triggered,
            [=]()
            {
                QMessageBox::information(this,"关于作者","作者:\tSeanLiao\n微信:\tyuan52\n华南师范大学");
            });
    scene->addPixmap(QPixmap(":/background/images/welcome_bg.jpg"));
    view->setScene(scene);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void MainWindow::initScene()
{
    scene->setSceneRect(0, 0,
                        static_cast<QWidget *>(scene->parent())->size().width(),
                        static_cast<QWidget *>(scene->parent())->size().height());
    
}

MainWindow::~MainWindow()
{

}
