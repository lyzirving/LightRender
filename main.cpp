#include <QtWidgets/QApplication>
#include <QtWidgets/qdesktopwidget.h>

#include "LrWidget.h"
#include "RrtTest.h"

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

    QDesktopWidget *desktopWidget = QApplication::desktop();
    const QRect &desktop = desktopWidget->availableGeometry();
    
    // todo: if the window is not square, the real-time ray tracing's output image is stretched
    int width = 500;
    int height = 500;
    int left = (int)((desktop.width() - width) * 0.5f);
    int top = (int)((desktop.height() - height) * 0.5f);

    // show ui
    LrWidget w;
    w.setGeometry(left, top, width, height);
    w.show();

    RrtTest::main();

    int ret = a.exec();

    // do some release
    onQuit();

    return ret;
}

