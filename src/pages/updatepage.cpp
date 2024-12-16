#include "updatepage.h"
#include "ui_updatepage.h"

updatepage::updatepage(QWidget *parent)
    : basepage("在线更新",parent)
    , ui(new Ui::updatepage)
{
    initalUpdatePage();
}

updatepage::~updatepage()
{
}

void updatepage::resetPage()
{
}

void updatepage::back()
{
}

void updatepage::initalUpdatePage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/update");
}
