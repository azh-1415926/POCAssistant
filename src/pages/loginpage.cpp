#include "loginpage.h"
#include "ui_loginpage.h"

#include "jsonFile.hpp"
#include "base.hpp"

#include <QTimer>

#define ERROR_TEXT(str) "<p><span style=\"color: rgb(231, 95, 51);\">" \
    str \
    "</span></p>"

loginpage::loginpage(QWidget *parent)
    : basepage(parent)
    , ui(new Ui::loginpage)
{
    initalLoginPage();
}

loginpage::~loginpage()
{
}

void loginpage::resetPage()
{
}

void loginpage::back()
{
}

void loginpage::toLogin(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&loginpage::toLogin);

    QString str(reply->readAll());
    qDebug()<<"reply:"<<str;

    // int begin=str.indexOf("{");
    // int end=str.lastIndexOf("}");
    
    jsonFile json;
    // json.fromJson(str.mid(begin,end+1-begin));
    json.fromJson(str);

    if(json.value("success").toString()=="true")
    {
        ui->textOfError->hide();
        // currToken=json.value("token").toString();
        userId::getInstance().set(ui->inputOfAccount->text());
        emit logon();
    }
    else if(json.value("success").toString()=="false")
    {
        ui->textOfError->setText(ERROR_TEXT("账号或密码错误"));
        ui->textOfError->show();
    }
    else
    {
        ui->textOfError->setText(ERROR_TEXT("登录超时"));
        ui->textOfError->show();
    }
}

void loginpage::initalLoginPage()
{
    ui->setupUi(this);

    ui->inputOfAccount->setPlaceholderText("请输入学号/职工号");
    ui->inputOfPassword->setPlaceholderText("请输入密码");

    ui->inputOfPassword->setEchoMode(QLineEdit::Password);

    ui->optionOfRole->addItems(QStringList()<<"学生"<<"教师"<<"管理员");
    ui->textOfError->hide();

    connect(ui->btnOfLogin,&QPushButton::clicked,this,[=]()
    {
        connect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&loginpage::toLogin);

        QNetworkRequest request;
        request.setUrl("http://127.0.0.1:8848/login/token?userId="+ui->inputOfAccount->text()+"&passwd="+ui->inputOfPassword->text());
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        QJsonObject obj;
        QJsonDocument doc(obj);

        httpManager::getInstance().get()->post(request,doc.toJson());
    });

    // QTimer* timer=new QTimer(this);
    // connect(timer,&QTimer::timeout,this,[=]()
    // {
    //     ui->textOfError->hide();
    // });
    // timer->start(15000);
}
