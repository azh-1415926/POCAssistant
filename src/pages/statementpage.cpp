#include "statementpage.h"
#include "ui_statementpage.h"

statementpage::statementpage(QWidget *parent)
    : basepage("在线统计",parent)
    , ui(new Ui::statementpage)
{
    initalStatementPage();
}

statementpage::~statementpage()
{
}

void statementpage::resetPage()
{
}

void statementpage::back()
{
}

void statementpage::initalStatementPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/statement");
}
