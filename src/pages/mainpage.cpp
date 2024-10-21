#include "mainpage.h"
#include "ui_mainpage.h"

mainpage::mainpage(QWidget *parent)
    : basepage("主页",parent)
    , ui(new Ui::mainpage)
{
    initalmainpage();
}

mainpage::~mainpage()
{
}

void mainpage::resetPage()
{
}

void mainpage::back()
{
}

void mainpage::selectLabel(const clicklabel* label)
{
    if(label==ui->course)
    {
        emit selectPage(1);
    }
    else if(label==ui->program)
    {
        emit selectPage(2);
    }
    else if(label==ui->quiz)
    {
        emit selectPage(3);
    }
    else
    {
        ;
    }
}

void mainpage::initalmainpage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/home");

    connect(ui->course,&clicklabel::clicked,this,&mainpage::selectLabel);
    connect(ui->program,&clicklabel::clicked,this,&mainpage::selectLabel);
    connect(ui->quiz,&clicklabel::clicked,this,&mainpage::selectLabel);

    connect(ui->btnOfCourse,&QPushButton::clicked,this,[=](){ selectPage(1); });
    connect(ui->btnOfProgram,&QPushButton::clicked,this,[=](){ selectPage(2); });
    connect(ui->btnOfQuiz,&QPushButton::clicked,this,[=](){ selectPage(3); });
}
