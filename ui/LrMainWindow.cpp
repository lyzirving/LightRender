#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qboxlayout.h>
#include <QtCore/qrect.h>

#include "LrMainWindow.h"
#include "LrWidget.h"

#include "RrtTest.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LrMainWindow"

LrMainWindow::LrMainWindow() : QMainWindow(), m_mainWidget(nullptr)
{
    QWidget* central = new QWidget;
    this->setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    m_mainWidget = new LrWidget;

    QPushButton* btn = new QPushButton("Btn0: print Ray Tracing");
    QFontMetrics metrics(btn->font());
    int txtWidth = metrics.boundingRect(btn->text()).width();
    int txtHeight = metrics.boundingRect(btn->text()).height();
    int padding = 10;
    btn->setFixedSize(txtWidth + padding, txtHeight + padding);

    mainLayout->addWidget(m_mainWidget);
    mainLayout->addWidget(btn);

    central->setLayout(mainLayout);

    connect(btn, SIGNAL(clicked()), this, SLOT(btn0Clicked()));
}

void LrMainWindow::btn0Clicked()
{
    //RrtTest::main();
}

void LrMainWindow::closeEvent(QCloseEvent* e)
{
    LOG_INFO("main window close");
    if (m_mainWidget)
    {
        m_mainWidget->quitThread();
    }
}