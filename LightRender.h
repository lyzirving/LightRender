#pragma once

#include <QtWidgets/QWidget>
#include "ui_LightRender.h"

class LightRender : public QWidget
{
    Q_OBJECT

public:
    LightRender(QWidget *parent = nullptr);
    ~LightRender();

private:
    Ui::LightRenderClass ui;
};
