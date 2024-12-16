#include "managementpage.h"
#include "ui_managementpage.h"

managementpage::managementpage(QWidget *parent)
    : basepage("在线管理",parent)
    , ui(new Ui::managementpage)
{
    initalManagementpage();
}

managementpage::~managementpage()
{
}

void managementpage::resetPage()
{
}

void managementpage::back()
{
}

void managementpage::initalManagementpage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/management");
}
