#include "LrWidget.h"
#include "Shader.h"
#include "Logger.h"

LrWidget::LrWidget(QWidget *parent) : QWidget(parent, Qt::MSWindowsOwnDC)
{
    Logger::init();

    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(true);

    ui.setupUi(this);
}

LrWidget::~LrWidget() 
{
    m_shader.reset();
}
 
QPaintEngine* LrWidget::paintEngine() const { return nullptr; }

void LrWidget::closeEvent(QCloseEvent* e)
{
    Logger::release();
}
