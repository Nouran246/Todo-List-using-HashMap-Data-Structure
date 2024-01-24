#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "HashMap.h"
#include <QFile>
#include <QToolButton>
#include <QToolBar>
#include <QLineEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HashMap map(100);

    MainWindow w;
    w.readFromFile(map.File);
    map.File.close();
    w.show();

    return a.exec();

}
