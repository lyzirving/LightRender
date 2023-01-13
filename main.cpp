#include "LightRender.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LightRender w;
    w.show();
    return a.exec();
}
