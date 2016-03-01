#include "mainwindow.h"
#include "variable.h"
#include <QApplication>

int variable::precision = 10;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
