#include "loginpage.h"
#include "ui_loginpage.h"

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

// 接收后端响应，判断是否登陆成功，成功便发送 logon 信号
void loginpage::toLogin(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&loginpage::toLogin);

    QString str(reply->readAll());
    azh::logger()<<"loginpage login:"<<str;
    
    jsonFile json;
    json.fromJson(str);

    if(json.value("result").toString()=="true")
    {
        ui->textOfError->hide();
        // 存储当前用户 id 到全局变量 userId 中，在后续操作中需要用到用户 id
        userId::getInstance().set(ui->inputOfAccount->text());

        // 管理员额外存储 token 数据到全局变量 tokenOfAdmin 中，管理员请求高级权限的 api 时需要带上 token
        QString token=json.value("token").toString();
        if(!token.isEmpty())
        {
            tokenOfAdmin::getInstance().set(token);
        }

        emit logon();
    }
    else if(json.value("result").toString()=="false")
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
    ui->optionOfRole->hide();
    ui->textOfError->hide();

    connect(ui->btnOfLogin,&QPushButton::clicked,this,[=]()
    {
        connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&loginpage::toLogin);

        QNetworkRequest request;
        request.setUrl(URL_OF_SERVER+"/User/login");
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        QJsonObject obj;
        obj.insert("id",ui->inputOfAccount->text());
        obj.insert("password",ui->inputOfPassword->text());
        QJsonDocument doc(obj);

        HTTP_MANAGER->post(request,doc.toJson());
    });
}
