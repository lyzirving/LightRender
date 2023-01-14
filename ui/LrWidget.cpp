#include "LrWidget.h"
#include "Shader.h"

LrWidget::LrWidget(QWidget *parent) : QWidget(parent, Qt::MSWindowsOwnDC)
{
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
}
