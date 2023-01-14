#include "LrWidget.h"
#include "GfxContext.h"
#include "GfxDevice.h"

LrWidget::LrWidget(QWidget *parent) : QWidget(parent, Qt::MSWindowsOwnDC), m_ctx(new GfxContext)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(true);

    ui.setupUi(this);

    m_ctx->bind(((HWND)winId()));

    GfxDevice::init();
}

LrWidget::~LrWidget() 
{
    m_ctx.reset();
}
 
QPaintEngine* LrWidget::paintEngine() const { return nullptr; }

void LrWidget::closeEvent(QCloseEvent* e)
{
}
