#include "quizpage.h"
#include "ui_quizpage.h"

quizpage::quizpage(QWidget *parent)
    : basepage("在线测试",parent)
    , ui(new Ui::quizpage)
{
    initalQuizPage();
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

void quizpage::initalQuizPage()
{
    ui->setupUi(this);

    connect(ui->btnOfTest,&QPushButton::clicked,this,[=]()
    {
        ui->stackedWidget->setCurrentIndex(1);
    });
}