#include "managementpage.h"
#include "ui_managementpage.h"

#include "jsonFile.hpp"

// 当前操作应当请求的 url
SINGLETONE(currUrl,QString)
// 当前提交按钮对应的操作
SINGLETONE(currOperation,operationOfManagement)
// 当前是否为导入操作
SINGLETONE(isLoad,bool)

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

    ui->idOfUser->clear();
    lockUserInfo(false);
    clearUserInfo();

    ui->idOfClass->clear();
    lockClassInfo(false);
    clearClassInfo();
}

void managementpage::back()
{
}

QString managementpage::getUrlByOperation(operationOfManagement op)
{
    QString url=URL_OF_SERVER;
    
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
        /* 班级查询 */
        url+="/Manager/getUser";
        break;
    case operationOfManagement::CLASS_SERACH:
        /* 班级查询 */
        url+="/Manager/getClass";
        break;
    
    case operationOfManagement::USER_INFO:
        /* 用户查询 */
        url+="/User/info";
        break;

    case operationOfManagement::CLASS_INFO:
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
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfUser);

    QString str=reply->readAll();
    
    qDebug()<<"operationOfUser:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        ui->textOfOutput->setText(json.value("info").toString());

        // 若为载入操作，更新控件，并锁住id输入框
        if(isLoad::getInstance().get())
        {
            if(result=="true")
            {
                ui->nameOfUser->setText(json.value("name").toString());
                ui->roleOfUser->setCurrentIndex(json.value("role").toInt());
                ui->passwdOfUser->setText(json.value("password").toString());

                ui->idOfUser->setEnabled(false);

                ui->loadOfUser->setText("取消载入");
            }
            
            isLoad::getInstance().set(false);

            return;
        }

        // 若为提交操作，则取消载入的数据
        if(result=="true")
        {
            loadUserInfo(false,currOperation::getInstance().get()==operationOfManagement::ALTER_USER?false:true);
        }
        else
        {
            lockUserInfo(false);
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
}

void managementpage::operationOfClass(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfClass);

    QString str=reply->readAll();
    
    qDebug()<<"operationOfClass:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        ui->textOfOutput->setText(json.value("info").toString());

        // 若为载入操作，更新控件，并锁住id输入框
        if(isLoad::getInstance().get())
        {
            if(result=="true")
            {
                ui->nameOfClass->setText(json.value("name").toString());
                ui->teacherOfClass->setText(json.value("teacherId").toString());

                ui->idOfClass->setEnabled(false);

                ui->loadOfClass->setText("取消载入");
            }
            
            isLoad::getInstance().set(false);

            return;
        }

        // 若为提交操作，则取消载入的数据
        if(result=="true")
        {
            loadClassInfo(false,currOperation::getInstance().get()==operationOfManagement::ALTER_CLASS?false:true);
        }
        else
        {
            lockClassInfo(false);
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
}

void managementpage::operationOfSearch(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfSearch);

    QString str=reply->readAll();
    
    qDebug()<<"operationOfSearch:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        ui->textOfOutput->setText(json.value("info").toString());

        if(result=="true")
        {
            int row=json.value("row").toInt();
            int col=json.value("col").toInt();
            ui->tableOfSearch->clear();
            ui->tableOfSearch->setRowCount(row-1);
            ui->tableOfSearch->setColumnCount(col);

            QStringList title;

            for(int i=0;i<col;i++)
            {
                QJsonObject recordOfTitle=json.value("0").toObject();
                title<<recordOfTitle.value(QString::number(i)).toString();
            }

            ui->tableOfSearch->setHorizontalHeaderLabels(title);

            for(int i=1;i<row;i++)
            {
                QJsonObject record=json.value(QString::number(i)).toObject();
                
                for(int j=0;j<col;j++)
                {
                    ui->tableOfSearch->setItem(i-1,j,new QTableWidgetItem(record.value(QString::number(j)).toString()));
                }
            }
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
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
        ui->pages->setCurrentIndex(i);
    });

    // 子功能就为当前操作
    connect(ui->optionsOfSubFunc,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        int index=0;

        for(int ii=0;ii<ui->optionsOfFunc->currentIndex();ii++)
        {
            index+=subOptions[ii].length();
        }

        index+=i;

        operationOfManagement op=(operationOfManagement)index;

        currOperation::getInstance().set(op);
        currUrl::getInstance().set(getUrlByOperation(op));

        if(op==operationOfManagement::ADD_USER)
        {
            ui->loadOfUser->hide();
            loadUserInfo(false);
        }
        else
        {
            ui->loadOfUser->show();
        }

        if(op==operationOfManagement::REMOVE_USER)
        {
            ui->labelOfPasswd->hide();
            ui->passwdOfUser->hide();
        }
        else
        {
            ui->labelOfPasswd->show();
            ui->passwdOfUser->show();
        }

        if(op==operationOfManagement::ALTER_USER)
        {
            ui->labelOfRole->hide();
            ui->roleOfUser->hide();
        }
        else
        {
            ui->labelOfRole->show();
            ui->roleOfUser->show();
        }

        if(op==operationOfManagement::ADD_CLASS)
        {
            ui->loadOfClass->hide();
        }
        else
        {
            ui->loadOfClass->show();
        }

        if(op==operationOfManagement::USER_SEARCH||op==operationOfManagement::CLASS_SERACH)
        {
            QJsonObject obj;
            postByCurrOperationWithData(2,obj);
        }
    });

    ui->optionsOfFunc->addItems(options);

    connect(ui->submitOfUser,&QPushButton::clicked,this,[=]()
    {
        if(ui->loadOfUser->text()!="取消载入"&&currOperation::getInstance().get()!=operationOfManagement::ADD_USER)
        {
            ui->textOfOutput->setText("请先载入用户信息");
            return;
        }

        if(ui->idOfUser->text().isEmpty()||ui->nameOfUser->text().isEmpty())
        {
            ui->textOfOutput->setText("未输入用户id与用户姓名，无法进行提交");
            return;
        }

        QJsonObject obj;
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

        postByCurrOperationWithData(0,obj);
    });

    connect(ui->submitOfClass,&QPushButton::clicked,this,[=]()
    {
        if(ui->loadOfClass->text()!="取消载入"&&currOperation::getInstance().get()!=operationOfManagement::ADD_CLASS)
        {
            ui->textOfOutput->setText("请先载入班级信息");
            return;
        }

        if(ui->idOfClass->text().isEmpty()||ui->nameOfClass->text().isEmpty()||ui->teacherOfClass->text().isEmpty())
        {
            ui->textOfOutput->setText("未输入班级id与班级名称、任课老师，无法进行提交");
            return;
        }

        QJsonObject obj;
        // id required
        obj.insert("id",ui->idOfClass->text());
        // 在添加班级、修改班级时使用，在删除班级时不使用
        if(currOperation::getInstance().get()!=operationOfManagement::REMOVE_CLASS)
        {
            obj.insert("name",ui->nameOfClass->text());
            obj.insert("teacherId",ui->teacherOfClass->text());
        }

        lockClassInfo(true);

        postByCurrOperationWithData(1,obj);
    });

    connect(ui->loadOfUser,&QPushButton::clicked,this,[=]()
    {
        if(ui->loadOfUser->text()=="载入")
        {
            loadUserInfo(true);
        }
        else
        {
            loadUserInfo(false);
        }
    });

    connect(ui->loadOfClass,&QPushButton::clicked,this,[=]()
    {
        if(ui->loadOfClass->text()=="载入")
        {
            loadClassInfo(true);
        }
        else
        {
            loadClassInfo(false);
        }
    });
}

void managementpage::loadUserInfo(bool needToLoad,bool needToClearId)
{
    if(needToClearId)
    {
        ui->idOfUser->clear();
    }

    // 取消载入
    if(!needToLoad)
    {
        ui->loadOfUser->setText("载入");
        ui->idOfUser->setEnabled(true);
        lockUserInfo(false);
        clearUserInfo();

        // ui->textOfOutput->setText("已取消载入");

        return;
    }
    
    // 载入成功后再锁定用户信息，并修改控件文本

    if(ui->idOfUser->text().isEmpty())
    {
        ui->textOfOutput->setText("未输入用户id，无法载入");
        return;
    }

    isLoad::getInstance().set(true);

    QJsonObject obj;
    // id required
    obj.insert("id",ui->idOfUser->text());

    postByOperationWithData(0,operationOfManagement::USER_INFO,obj);
}

void managementpage::loadClassInfo(bool needToLoad, bool needToClearId)
{
    if(needToClearId)
    {
        ui->idOfClass->clear();
    }

    // 取消载入
    if(!needToLoad)
    {
        ui->loadOfClass->setText("载入");
        ui->idOfClass->setEnabled(true);
        lockClassInfo(false);
        clearClassInfo();

        // ui->textOfOutput->setText("已取消载入");

        return;
    }
    
    // 载入成功后再锁定信息，并修改控件文本

    if(ui->idOfClass->text().isEmpty())
    {
        ui->textOfOutput->setText("未输入班级id，无法载入");
        return;
    }

    isLoad::getInstance().set(true);

    QJsonObject obj;
    // id required
    obj.insert("id",ui->idOfClass->text());

    postByOperationWithData(1,operationOfManagement::CLASS_INFO,obj);
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
    ui->nameOfUser->clear();
    ui->roleOfUser->setCurrentIndex(0);
    ui->passwdOfUser->clear();
}

void managementpage::clearClassInfo()
{
    ui->nameOfClass->clear();
    ui->teacherOfClass->clear();
}

void managementpage::postByOperationWithData(int funcId, const operationOfManagement &op, const QJsonObject &obj)
{
    switch (funcId)
    {
    case 0:
        connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfUser);
        break;
    case 1:
        connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfClass);
        break;
    case 2:
        connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfSearch);
        break;
    
    default:
        break;
    }

    QNetworkRequest request;
    request.setUrl(getUrlByOperation(op));
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject data=obj;

    // token required
    data.insert("token",tokenOfAdmin::getInstance().get());

    QJsonDocument doc(data);

    HTTP_MANAGER->post(request,doc.toJson());
}

void managementpage::postByCurrOperationWithData(int funcId, const QJsonObject &obj)
{
    postByOperationWithData(funcId,currOperation::getInstance().get(),obj);
}