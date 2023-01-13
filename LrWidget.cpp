#include "LrWidget.h"
#include "Shader.h"

#include <glog/logging.h>
#pragma comment(lib, "glog.lib")

LrWidget::LrWidget(QWidget *parent) : QWidget(parent, Qt::MSWindowsOwnDC)
{
    //todo wrap by log util;
    google::InitGoogleLogging("LightRender");
    FLAGS_alsologtostderr = true;//是否将日志输出到文件和stderr
    FLAGS_colorlogtostderr = true;//是否启用不同颜色显示

    google::SetLogDestination(google::GLOG_INFO, "D:\\Visual Studio Project\\LightRender\\logs\\INFO_");//INFO级别的日志都存放到logs目录下且前缀为INFO_
    google::SetLogDestination(google::GLOG_WARNING, "D:\\Visual Studio Project\\LightRender\\logs\\WARNING_");//WARNING级别的日志都存放到logs目录下且前缀为WARNING_
    google::SetLogDestination(google::GLOG_ERROR, "D:\\Visual Studio Project\\LightRender\\logs\\ERROR_");	//ERROR级别的日志都存放到logs目录下且前缀为ERROR_
    google::SetLogDestination(google::GLOG_FATAL, "D:\\Visual Studio Project\\LightRender\\logs\\FATAL_");	//FATAL级别的日志都存放到logs目录下且前缀为FATAL_

    m_shader = std::make_shared<Shader>();
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
    google::ShutdownGoogleLogging();
}
