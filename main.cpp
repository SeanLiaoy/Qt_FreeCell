#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <QMessageBox>
#include <QDesktopWidget>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");


    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
