#pragma once

#include <QtWidgets/QWidget>
#include <memory>

#include "ui_LightRender.h"

class GreThread;

class LrWidget : public QWidget
{
    Q_OBJECT

public:
    LrWidget(QWidget *parent = nullptr);
    virtual ~LrWidget();

    virtual QPaintEngine* paintEngine() const override;

    void quitThread();

protected:
    virtual void closeEvent(QCloseEvent* e) override;
    virtual void resizeEvent(QResizeEvent* event) override;

    std::shared_ptr<GreThread> m_renderThread;

private:
    Ui::LrWidgetClass ui;
};
