#include "LrWidget.h"
#include "Shader.h"

#include <glog/logging.h>
#pragma comment(lib, "glog.lib")

LrWidget::LrWidget(QWidget *parent) : QWidget(parent, Qt::MSWindowsOwnDC)
{
    //todo wrap by log util;
    google::InitGoogleLogging("LightRender");
    FLAGS_alsologtostderr = true;//�Ƿ���־������ļ���stderr
    FLAGS_colorlogtostderr = true;//�Ƿ����ò�ͬ��ɫ��ʾ

    google::SetLogDestination(google::GLOG_INFO, "D:\\Visual Studio Project\\LightRender\\logs\\INFO_");//INFO�������־����ŵ�logsĿ¼����ǰ׺ΪINFO_
    google::SetLogDestination(google::GLOG_WARNING, "D:\\Visual Studio Project\\LightRender\\logs\\WARNING_");//WARNING�������־����ŵ�logsĿ¼����ǰ׺ΪWARNING_
    google::SetLogDestination(google::GLOG_ERROR, "D:\\Visual Studio Project\\LightRender\\logs\\ERROR_");	//ERROR�������־����ŵ�logsĿ¼����ǰ׺ΪERROR_
    google::SetLogDestination(google::GLOG_FATAL, "D:\\Visual Studio Project\\LightRender\\logs\\FATAL_");	//FATAL�������־����ŵ�logsĿ¼����ǰ׺ΪFATAL_

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
