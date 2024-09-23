#include "loginpage.h"
#include "ui_loginpage.h"

#include "jsonFile.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>

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

void loginpage::initalLoginPage()
{
    ui->setupUi(this);

    ui->inputOfAccount->setPlaceholderText("请输入学号/职工号");
    ui->inputOfPassword->setPlaceholderText("请输入密码");

    ui->inputOfPassword->setEchoMode(QLineEdit::Password);

    ui->optionOfRole->addItems(QStringList()<<"学生"<<"教师"<<"管理员");
    
    static QNetworkAccessManager* manager=new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply* reply){
        QString str(reply->readAll());
        qDebug()<<"reply:"<<str;

        int begin=str.indexOf("{");
        int end=str.lastIndexOf("}");
        
        jsonFile json;
        json.fromJson(str.mid(begin,end+1-begin));

        if(json.value("success").toString()=="true")
        {
            emit logon(json.value("token").toString());
        }
    });

    connect(ui->btnOfLogin,&QPushButton::clicked,this,[=]()
    {
        QNetworkRequest request;
        request.setUrl("http://127.0.0.1:8848/login/token?userId="+ui->inputOfAccount->text()+"&passwd="+ui->inputOfPassword->text());
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        QJsonObject obj;
        QJsonDocument doc(obj);

        manager->post(request,doc.toJson());

        emit logon("");
    });
}
