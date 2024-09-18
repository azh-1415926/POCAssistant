#include "quizpage.h"
#include "ui_quizpage.h"

quizpage::quizpage(QWidget *parent)
    : basepage("在线测试",parent)
    , ui(new Ui::quizpage)
{
    initalquizpage();
}

quizpage::~quizpage()
{
}

void quizpage::resetPage()
{
}

void quizpage::back()
{
}

void quizpage::initalquizpage()
{
    ui->setupUi(this);
}