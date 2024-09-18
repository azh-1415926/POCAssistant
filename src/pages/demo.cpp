#include "demo.h"
#include "ui_demo.h"

demo::demo(QWidget *parent)
    : basepage(parent)
    , ui(new Ui::demo)
{
    initaldemo();
}

demo::~demo()
{
}

void demo::resetPage()
{
}

void demo::back()
{
}

void demo::initaldemo()
{
    ui->setupUi(this);
}
