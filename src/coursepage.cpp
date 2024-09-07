#include "coursepage.h"
#include "ui_coursepage.h"

coursepage::coursepage(basepage *parent)
    : basepage(parent)
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

    setStatus("课程学习","");
}
