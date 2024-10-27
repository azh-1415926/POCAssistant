#include "quizpage.h"
#include "ui_quizpage.h"

#include "quizcontroller.h"
#include "quizmodel.h"
#include "jsonFile.hpp"

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

    quizcontroller* controller=new quizcontroller(this);
    quizmodel* model=new quizmodel(this);

    controller->bind(ui->pageOfTest);
    controller->bind(model);

    jsonFile file;
    file.load("quiz.json");
    model->load(file.toJson());
}