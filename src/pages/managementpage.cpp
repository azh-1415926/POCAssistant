#include "managementpage.h"
#include "ui_managementpage.h"

#include "jsonFile.hpp"

// 当前操作应当请求的 url
SINGLETONE(currUrl,QString)
// 当前操作
SINGLETONE(currOperation,operationOfManagement)

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

QString managementpage::getUrlByOperation(operationOfManagement op)
{
    QString url="http://"
        SERVER_IP
        ":"
        SERVER_PORT_S
    ;

    switch (op)
    {
    case operationOfManagement::ADD_USER:
        /* 添加用户 */
        url+="/Manager/addUser";
        break;

    case operationOfManagement::REMOVE_USER:
        /* 删除用户 */
        url+="/Manager/removeUser";
        break;
        
    case operationOfManagement::ALTER_USER:
        /* 修改用户 */
        url+="/Manager/alterUser";
        break;

    case operationOfManagement::ADD_CLASS:
        /* 添加班级 */
        url+="/Manager/addClass";
        break;
    
    case operationOfManagement::REMOVE_CLASS:
        /* 删除班级 */
        url+="/Manager/removeClass";
        break;

    case operationOfManagement::ALTER_CLASS:
        /* 修改班级 */
        url+="/Manager/alterClass";
        break;

    case operationOfManagement::CLASS_ALLOC:
        /* 班级分配 */
        break;
    
    case operationOfManagement::USER_SEARCH:
        /* 用户查询 */
        url+="/User/info";
        break;

    case operationOfManagement::CLASS_SERACH:
        /* 班级查询 */
        url+="/Class/info";
        break;

    default:
        break;
    }

    return url;
}

void managementpage::operationOfUser(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&managementpage::operationOfUser);

    QString str=reply->readAll();
    ui->textOfOutput->setText(str);

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        ui->textOfOutput->setText(json.value("info").toString());

        if(result=="true")
        {
            clearUserInfo();
        }
    }
    else
    {
        emit logoff();
    }

    lockUserInfo(false);
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

    QStringList options={"用户管理","班级管理","查询"};
    QList<QStringList> subOptions=
    {
        QStringList()<<"添加用户"<<"删除用户"<<"修改用户",
        QStringList()<<"添加班级"<<"删除班级"<<"修改班级"<<"班级分配",
        QStringList()<<"用户查询"<<"班级查询"
    };

    connect(ui->optionsOfFunc,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        ui->optionsOfSubFunc->clear();
        ui->optionsOfSubFunc->addItems(subOptions[i]);
    });

    // 子功能就为当前操作
    connect(ui->optionsOfSubFunc,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        int index=0;

        for(int ii=0;ii<i;ii++)
        {
            index+=subOptions[i].length();
        }

        operationOfManagement op=(operationOfManagement)index;

        currOperation::getInstance().set(op);
        currUrl::getInstance().set(getUrlByOperation(op));

        if(op==operationOfManagement::ADD_USER)
        {
            ui->loadOfUser->hide();
        }
        else
        {
            ui->loadOfUser->show();
        }

        if(op==operationOfManagement::ADD_CLASS)
        {
            ui->loadOfClass->hide();
        }
        else
        {
            ui->loadOfClass->show();
        }
    });

    ui->optionsOfFunc->addItems(options);

    connect(ui->submitOfUser,&QPushButton::clicked,this,[=]()
    {
        if(ui->idOfUser->text().isEmpty()||ui->nameOfUser->text().isEmpty())
        {
            ui->textOfOutput->setText("未输入用户id与用户姓名，无法进行提交");
            return;
        }

        if(!userInfoIsLock&&currOperation::getInstance().get()!=operationOfManagement::ADD_USER)
        {
            ui->textOfOutput->setText("请先载入用户信息");
            return;
        }

        connect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&managementpage::operationOfUser);

        QNetworkRequest request;
        request.setUrl(QUrl(currUrl::getInstance().get()));
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        QJsonObject obj;

        // token required
        obj.insert("token",tokenOfAdmin::getInstance().get());
        // id required
        obj.insert("id",ui->idOfUser->text());
        // 在添加用户、修改用户时使用，在删除用户时不使用
        if(currOperation::getInstance().get()!=operationOfManagement::REMOVE_USER)
        {
            obj.insert("name",ui->nameOfUser->text());
            obj.insert("role",ui->roleOfUser->currentIndex());
            obj.insert("password",ui->passwdOfUser->text().isEmpty()?"123456":ui->passwdOfUser->text());
        }

        QJsonDocument doc(obj);

        lockUserInfo(true);

        httpManager::getInstance().get()->post(request,doc.toJson());
    });

    connect(ui->loadOfUser,&QPushButton::clicked,this,[=]()
    {
        if(userInfoIsLock&&ui->loadOfUser->text()=="取消载入")
        {
            clearUserInfo();
            ui->textOfOutput->setText("已取消");
            return;
        }

        if(ui->idOfUser->text().isEmpty()||ui->nameOfUser->text().isEmpty())
        {
            ui->textOfOutput->setText("未输入用户id，无法载入");
            return;
        }

        connect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&managementpage::operationOfUser);

        QNetworkRequest request;
        request.setUrl(QUrl(currUrl::getInstance().get()));
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        QJsonObject obj;

        // token required
        obj.insert("token",tokenOfAdmin::getInstance().get());
        // id required
        obj.insert("id",ui->idOfUser->text());

        QJsonDocument doc(obj);

        httpManager::getInstance().get()->post(request,doc.toJson());
    });
}

void managementpage::lockUserInfo(bool shouldLock)
{
    userInfoIsLock=shouldLock;
    if(shouldLock)
    {
        ui->nameOfUser->setEnabled(false);
        ui->roleOfUser->setEnabled(false);
        ui->passwdOfUser->setEnabled(false);
    }
    else
    {
        ui->nameOfUser->setEnabled(true);
        ui->roleOfUser->setEnabled(true);
        ui->passwdOfUser->setEnabled(true);
    }
}

void managementpage::lockClassInfo(bool shouldLock)
{
    classInfoIsLock=shouldLock;
    if(shouldLock)
    {
        ui->nameOfClass->setEnabled(false);
        ui->teacherOfClass->setEnabled(false);
    }
    else
    {
        ui->nameOfClass->setEnabled(true);
        ui->teacherOfClass->setEnabled(true);
    }
}

void managementpage::clearUserInfo()
{
    ui->nameOfUser->setText("");
    ui->roleOfUser->setCurrentIndex(0);
    ui->passwdOfUser->setText("");
}
