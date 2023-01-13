#pragma once

#include <QtWidgets/QWidget>
#include "ui_LightRender.h"

class LrWidget : public QWidget
{
    Q_OBJECT

public:
    LrWidget(QWidget *parent = nullptr);
    ~LrWidget();

private:
    Ui::LrWidgetClass ui;
};
