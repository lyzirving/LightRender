#include <QtGui/qevent.h>

#include "LrWidget.h"
#include "GreThread.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LrWidget"

LrWidget::LrWidget(QWidget *parent) : QWidget(parent, Qt::MSWindowsOwnDC), m_renderThread(new GreThread("light render", 60, RenderType::RRT_CANVAS))
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(true);
    ui.setupUi(this);
    
    LOG_INFO("window pos[%d, %d], size[%d, %d]", this->x(), this->y(), this->width(), this->height());
    m_renderThread->setWindowInfo(((HWND)winId()), 0, 0, this->width(), this->height());
    m_renderThread->start();
}

LrWidget::~LrWidget() 
{
    m_renderThread.reset();
}
 
QPaintEngine* LrWidget::paintEngine() const { return nullptr; }

void LrWidget::closeEvent(QCloseEvent* e)
{
    quitThread();
}

void LrWidget::resizeEvent(QResizeEvent* event)
{
    if (m_renderThread)
    {
        m_renderThread->onWindowSizeChange(event->size().width(), event->size().height());
    }
}

void LrWidget::quitThread()
{
    if (m_renderThread)
    {
        m_renderThread->interrupt();
        m_renderThread->join();
    }
}
