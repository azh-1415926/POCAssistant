#include "coursepage.h"
#include "ui_coursepage.h"

coursepage::coursepage(QWidget *parent)
    : basepage("在线学习",parent)
    , ui(new Ui::coursepage)
{
    initalCoursePage();
}

coursepage::~coursepage()
{
}

void coursepage::resetPage()
{
}

void coursepage::back()
{
}

void coursepage::initalCoursePage()
{
    ui->setupUi(this);
}
