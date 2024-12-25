#include "managementpage.h"
#include "ui_managementpage.h"

#include <QJsonObject>
#include <QJsonDocument>

SINGLETONE(urlOfOperation,QString)
SINGLETONE(indexOfOperation,int)

QStringList options={"用户管理","班级管理","查询"};
QList<QStringList> subOptions=
{
    QStringList()<<"添加用户"<<"删除用户"<<"修改用户",
    QStringList()<<"添加班级"<<"删除班级"<<"修改班级"<<"班级分配",
    QStringList()<<"用户查询"<<"班级查询"
};

managementpage::managementpage(QWidget *parent)
    : basepage("在线管理",parent)
    , ui(new Ui::managementpage)
{
    initalManagementPage();
}

managementpage::~managementpage()
{
}

void managementpage::resetPage()
{
    ui->optionsOfFunc->setCurrentIndex(0);
    ui->optionsOfSubFunc->setCurrentIndex(0);
}

void managementpage::back()
{
}

QString managementpage::getUrlByOperation(int index)
{
    QString url="http://"
        SERVER_IP
        ":"
        SERVER_PORT_S
        "/Manager/"
    ;

    switch (index)
    {
    case 0:
        /* 添加用户 */
        url+="addUser";
        break;

    case 1:
        /* 删除用户 */
        break;
        
    case 2:
        /* 修改用户 */
        break;

    case 3:
        /* 添加班级 */
        break;
    
    case 4:
        /* 删除班级 */
        break;

    case 5:
        /* 修改班级 */
        break;

    case 6:
        /* 班级分配 */
        break;
    
    case 7:
        /* 用户查询 */
        break;

    case 8:
        /* 班级查询 */
        break;

    default:
        break;
    }

    return url;
}

void managementpage::operationOfUser(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&managementpage::operationOfUser);

    ui->textOfOutput->setText(reply->readAll());
}

void managementpage::operationOfClass(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&managementpage::operationOfClass);
}

void managementpage::operationOfSearch(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&managementpage::operationOfSearch);
}

void managementpage::initalManagementPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/management");

    connect(ui->optionsOfFunc,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        ui->optionsOfSubFunc->clear();
        ui->optionsOfSubFunc->addItems(subOptions[i]);
    });

    connect(ui->optionsOfSubFunc,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        int index=0;

        for(int ii=0;ii<i;ii++)
        {
            index+=subOptions[i].length();
        }

        indexOfOperation::getInstance().set(index);
        urlOfOperation::getInstance().set(getUrlByOperation(index));
    });

    ui->optionsOfFunc->addItems(options);

    connect(ui->submitOfUser,&QPushButton::clicked,this,[=]()
    {
        connect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&managementpage::operationOfUser);

        QNetworkRequest request;
        request.setUrl(QUrl(urlOfOperation::getInstance().get()));
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        if(ui->idOfUser->text().isEmpty()||ui->nameOfUser->text().isEmpty())
        {
            ui->textOfOutput->setText("未输入用户id与用户姓名，无法进行提交");
            return;
        }

        QJsonObject obj;

        obj.insert("token",tokenOfAdmin::getInstance().get());
        obj.insert("id",ui->idOfUser->text());
        obj.insert("name",ui->nameOfUser->text());
        obj.insert("role",ui->roleOfUser->currentIndex());
        obj.insert("password",ui->passwdOfUser->text().isEmpty()?"123456":ui->passwdOfUser->text());

        QJsonDocument doc(obj);

        httpManager::getInstance().get()->post(request,doc.toJson());
    });
}