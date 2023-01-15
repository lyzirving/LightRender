#include <QtGui/qevent.h>

#include "LrWidget.h"
#include "GfxContext.h"
#include "GfxDevice.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LrWidget"

LrWidget::LrWidget(QWidget *parent) : QWidget(parent, Qt::MSWindowsOwnDC), m_ctx(new GfxContext)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(true);

    ui.setupUi(this);

    m_ctx->bind(((HWND)winId()));
    
    LOG_INFO("window pos[%d, %d], size[%d, %d]", this->x(), this->y(), this->width(), this->height());
    GfxDevice::setViewport(0, 0, this->width(), this->height());
}

LrWidget::~LrWidget() 
{
    m_ctx.reset();
}
 
QPaintEngine* LrWidget::paintEngine() const { return nullptr; }

void LrWidget::closeEvent(QCloseEvent* e)
{
}

void LrWidget::resizeEvent(QResizeEvent* event)
{
    LOG_INFO("resize[%d, %d]", event->size().width(), event->size().height());
    GfxDevice::setViewport(0, 0, event->size().width(), event->size().height());
}
