#include "loginpage.h"
#include "ui_loginpage.h"

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

    connect(ui->btnOfLogin,&QPushButton::clicked,this,[=]()
    {
        emit logon();
    });
}
