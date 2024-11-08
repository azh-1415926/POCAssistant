#include "quizpage.h"
#include "ui_quizpage.h"

#include "quizcontroller.h"
#include "quizmodel.h"
#include "jsonFile.hpp"
#include "base.hpp"
#include "constant.h"

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
    ui->stackedWidget->setCurrentIndex(0);
}

void quizpage::back()
{
    int currIndex=ui->stackedWidget->currentIndex();

    if(currIndex>=1&&currIndex<4)
        ui->stackedWidget->setCurrentIndex(0);
}

void quizpage::updateTest(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&quizpage::updateTest);

    QString str(reply->readAll());
    qDebug()<<"getquiz:"<<str;

    jsonFile json;
    json.fromJson(str);

    modelOfTest->load(json.toJson());
}

void quizpage::updateCollection(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&quizpage::updateCollection);
}

void quizpage::updateWrong(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&quizpage::updateWrong);
}

void quizpage::initalQuizPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/quiz");

    quizcontroller* controller=new quizcontroller(this);
    modelOfTest=new quizmodel(this);

    controller->bind(ui->pageOfQuestions);
    controller->bind(modelOfTest);

    connect(ui->btnOfTest,&QPushButton::clicked,this,[=]()
    {
        connect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&quizpage::updateTest);

        QNetworkRequest request;
        request.setUrl(QUrl("http://"
        SERVER_IP
        ":"
        SERVER_PORT_S
        "/quiz/getquiz?chapter=0"));
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        httpManager::getInstance().get()->get(request);

        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->btnOfWrong,&QPushButton::clicked,this,[=]()
    {
        ui->stackedWidget->setCurrentIndex(2);
    });

    jsonFile file;
    file.load(":/json/test/quiz.json");
    modelOfTest->load(file.toJson());

    connect(ui->btnOfSwitch,&QPushButton::clicked,this,[=]()
    {
        if(ui->btnOfSwitch->text()=="切换至收藏页")
        {
            ui->btnOfSwitch->setText("切换至答题页");
            ui->pagesOfTest->setCurrentIndex(1);
        }
        else
        {
            ui->btnOfSwitch->setText("切换至收藏页");
            ui->pagesOfTest->setCurrentIndex(0);
        }
    });

    ui->frame3->hide();
}