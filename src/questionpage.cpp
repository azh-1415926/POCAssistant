#include "questionpage.h"
#include "ui_questionpage.h"

questionpage::questionpage(basepage *parent)
    : basepage(parent)
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

    setStatus("课后习题","");
}