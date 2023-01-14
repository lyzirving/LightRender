#include <QtWidgets/QApplication>

#include "LrWidget.h"
#include "Logger.h"

void onInit()
{
    Logger::init();
}

void onQuit()
{
    Logger::release();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // do some initialization
    onInit();

    // show ui
    LrWidget w;
    w.show();
    int ret = a.exec();

    // do some release
    onQuit();

    return ret;
}

