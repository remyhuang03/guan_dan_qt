#pragma once

#include <QtWidgets/QWidget>
#include "ui_guan_dan.h"
#define SCREEN_W 960
#define SCREEN_H 540

class guan_dan : public QWidget
{
    Q_OBJECT

public:
    guan_dan(QWidget *parent = nullptr);
    ~guan_dan();

private:
    Ui::guan_danClass ui;
};
