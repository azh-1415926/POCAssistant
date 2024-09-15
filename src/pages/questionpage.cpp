#include "questionpage.h"
#include "ui_questionpage.h"

questionpage::questionpage(basepage *parent)
    : basepage("在线测试",parent)
    , ui(new Ui::questionpage)
{
    initalQuestionPage();
}

questionpage::~questionpage()
{
}

void questionpage::resetPage()
{
}

void questionpage::back()
{
}

void questionpage::initalQuestionPage()
{
    ui->setupUi(this);
}