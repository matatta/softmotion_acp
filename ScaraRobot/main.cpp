#include "stdafx.h"
#include "scararobot.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScaraRobot w;
    w.show();
    return a.exec();
}