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
    int currIndex=ui->stackedWidget->currentIndex();

    if(currIndex>=1&&currIndex<4)
        ui->stackedWidget->setCurrentIndex(0);
}

void quizpage::initalQuizPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/quiz");

    connect(ui->btnOfTest,&QPushButton::clicked,this,[=]()
    {
        ui->stackedWidget->setCurrentIndex(1);
    });
}