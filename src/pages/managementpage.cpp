#include "managementpage.h"
#include "ui_managementpage.h"

// 当前操作应当请求的 url
SINGLETONE(currUrl,QString)
// 当前提交按钮对应的操作
SINGLETONE(currOperation,operationOfManagement)
// 当前是否为导入操作
SINGLETONE(isLoad,bool)

// 隐藏布局中的全部控件，status 为 true 时隐藏
void hideLayout(QLayout* layout,bool status)
{
    for(auto i =0;i<layout->count();i++)
    {
        QWidget *w = layout->itemAt(i)->widget();
        if(w != nullptr)
        {
            w->setVisible(!status);
        } 
    }
}

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
    // 重置下拉框的状态
    ui->optionOfFunc->setCurrentIndex(0);
    ui->optionOfSubFunc->setCurrentIndex(0);

    // 清空用户 id 输入框、信息展示框，解锁信息显示框的锁定
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

// 获取传入操作（operationOfManagement）的后端请求 url
QString managementpage::getUrlByOperation(operationOfManagement op)
{
    // 从全局变量 urlOfServer 中取出 url 前缀
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
        url+="/Class/allocate";
        break;

    case operationOfManagement::USER_SEARCH:
        /* 班级查询 */
        url+="/Manager/getUser";
        break;
    case operationOfManagement::CLASS_SERACH:
        /* 班级查询 */
        url+="/Manager/getClass";
        break;

    case operationOfManagement::USER_BATCH:
        /* 用户批量处理 */
        url+="/Manager/batchUser";
        break;
    case operationOfManagement::CLASS_BATCH:
        /* 班级批量处理 */
        url+="/Manager/batchClass";
        break;
    
    case operationOfManagement::USER_INFO:
        /* 用户查询 */
        url+="/User/info";
        break;

    case operationOfManagement::CLASS_INFO:
        /* 班级查询 */
        url+="/Class/info";
        break;

    case operationOfManagement::UNALLOC_USER:
        /* 未分配班级用户查询 */
        url+="/User/getUnallocatedStudent";
        break;

    default:
        break;
    }

    return url;
}

// 接收用户管理操作的响应
void managementpage::operationOfUser(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfUser);

    QString str=reply->readAll();
    
    azh::logger()<<"managementpage operationOfUser:"<<str;

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
                // 更新用户信息
                ui->nameOfUser->setText(json.value("name").toString());
                ui->roleOfUser->setCurrentIndex(json.value("role").toInt());
                ui->passwdOfUser->setText(json.value("password").toString());

                // 锁住 id 输入框
                ui->idOfUser->setEnabled(false);

                // 更新载入按钮的文本，当前为已载入，再次点击即可取消载入
                ui->btnOfLoadUser->setText("取消载入");
            }
            
            // 重置 isLoad 的值
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

// 接收班级管理操作的响应
void managementpage::operationOfClass(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfClass);

    QString str=reply->readAll();
    
    azh::logger()<<"managementpage operationOfClass:"<<str;

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
                // 更新班级信息
                ui->nameOfClass->setText(json.value("name").toString());
                ui->teacherOfClass->setText(json.value("teacherId").toString());

                // 锁住班级 id 输入框
                ui->idOfClass->setEnabled(false);

                // 修改班级载入按钮的文本
                ui->btnOfLoadClass->setText("取消载入");
            }
            
            // 重置 isLoad 的值
            isLoad::getInstance().set(false);

            return;
        }

        // 若为提交操作，则取消载入的数据
        if(result=="true")
        {
            // 若为班级分配操作，执行成功后需要请求未分配班级的用户列表
            if(currOperation::getInstance().get()==operationOfManagement::CLASS_ALLOC)
            {
                QJsonObject obj;
                postByOperationWithData(4,operationOfManagement::UNALLOC_USER,obj);

                return;
            }

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

// 接收查询操作的响应
void managementpage::operationOfSearch(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfSearch);

    QString str=reply->readAll();
    
    azh::logger()<<"managementpage operationOfSearch:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        ui->textOfOutput->setText(json.value("info").toString());

        if(result=="true")
        {
            // 更新 tableOfSearch 控件，使用获取到的数据，填充进表格里
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

// 接收批处理操作的响应
void managementpage::operationOfBatch(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfBatch);

    QString str=reply->readAll();
    
    azh::logger()<<"managementpage operationOfBatch:"<<str;

    jsonFile json;
    json.fromJson(str);

    ui->textOfOutput->setText(json.value("info").toString());
    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        if(result=="true")
        {
            ;
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
}

// 接收响应，更新未分配班级的用户列表
void managementpage::loadUnallocatedStudent(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::loadUnallocatedStudent);

    QString str=reply->readAll();
    
    azh::logger()<<"managementpage loadUnallocatedStudent:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        ui->textOfOutput->setText(json.value("info").toString());

        if(result=="true")
        {
            // 使用得到的数据更新 userTable 控件
            int count=json.value("count").toInt();

            ui->userTable->clear();
            ui->userTable->setRowCount(count);
            ui->userTable->setColumnCount(1);

            ui->userTable->setHorizontalHeaderLabels(QStringList()<<"未分配班级的用户id");

            for(int i=0;i<count;i++)
            {                
                ui->userTable->setItem(i,0,new QTableWidgetItem(json.value(QString::number(i)).toString()));
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

    // 三个模块，用户管理、班级管理、查询，对于若干子模块
    QStringList options={"用户管理","班级管理","查询","批量处理"};
    QList<QStringList> subOptions=
    {
        QStringList()<<"添加用户"<<"删除用户"<<"修改用户",
        QStringList()<<"添加班级"<<"删除班级"<<"修改班级"<<"班级分配",
        QStringList()<<"用户查询"<<"班级查询",
        QStringList()<<"用户批处理"<<"班级批处理"
    };

    connect(ui->optionOfFunc,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        // 模块的切换将更新 optionOfSubFunc，即子模块列表，并切换管理页面
        ui->optionOfSubFunc->clear();
        ui->optionOfSubFunc->addItems(subOptions[i]);
        ui->pages->setCurrentIndex(i);
    });

    connect(ui->optionOfSubFunc,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        if(i<0)
            return;
            
        int index=0;

        for(int ii=0;ii<ui->optionOfFunc->currentIndex();ii++)
        {
            index+=subOptions[ii].length();
        }

        index+=i;

        // 子功能在 subOptions 中的下标就为当前操作的枚举值
        operationOfManagement op=(operationOfManagement)index;

        // 设置全局变量 currOperation、currUrl，分别对于当前子模块的操作、当前触发操作对应的请求 url
        currOperation::getInstance().set(op);
        currUrl::getInstance().set(getUrlByOperation(op));

        // 添加用户操作需要隐藏载入用户按钮
        if(op==operationOfManagement::ADD_USER)
        {
            ui->btnOfLoadUser->hide();
            loadUserInfo(false);
        }
        else
        {
            ui->btnOfLoadUser->show();
        }

        // 删除用户操作需要隐藏密码输入框
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

        // 修改用户操作需要隐藏用户类型展示框，用户类型一经创建无法修改
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

        // 添加班级操作需要隐藏载入班级按钮
        if(op==operationOfManagement::ADD_CLASS)
        {
            ui->btnOfLoadClass->hide();
            loadClassInfo(false);
        }
        else
        {
            ui->btnOfLoadClass->show();
        }

        // 班级分配操作需要隐藏载入班级管理模块的控件，并请求未分配班级的用户列表
        if(op==operationOfManagement::CLASS_ALLOC)
        {
            hideLayout(ui->layoutOfUserTable,false);
            hideLayout(ui->layoutOfAlloc,false);
            ui->frameOfClass->hide();

            QJsonObject obj;
            postByOperationWithData(4,operationOfManagement::UNALLOC_USER,obj);
        }
        else
        {
            hideLayout(ui->layoutOfUserTable,true);
            hideLayout(ui->layoutOfAlloc,true);
            ui->frameOfClass->show();
        }

        // 查询操作需要立即请求子操作对应的数据
        if(op==operationOfManagement::USER_SEARCH||op==operationOfManagement::CLASS_SERACH)
        {
            QJsonObject obj;
            postByCurrOperationWithData(2,obj);
        }
    });

    // 设置子操作列表控件内容
    ui->optionOfFunc->addItems(options);

    // 用户管理界面的提交按钮
    connect(ui->btnOfUserManagement,&QPushButton::clicked,this,[=]()
    {
        // 若未载入用户数据且不为添加用户，则返回
        if(ui->btnOfLoadUser->text()!="取消载入"&&currOperation::getInstance().get()!=operationOfManagement::ADD_USER)
        {
            ui->textOfOutput->setText("请先载入用户信息");
            return;
        }

        // id 输入框、名字输入框为空则返回
        if(ui->idOfUser->text().isEmpty()||ui->nameOfUser->text().isEmpty())
        {
            ui->textOfOutput->setText("未输入用户id与用户姓名，无法进行提交");
            return;
        }

        QJsonObject obj;
        obj.insert("id",ui->idOfUser->text());
        // 在添加用户、修改用户时使用，在删除用户时不使用以下字段的数据
        if(currOperation::getInstance().get()!=operationOfManagement::REMOVE_USER)
        {
            obj.insert("name",ui->nameOfUser->text());
            obj.insert("role",ui->roleOfUser->currentIndex());
            obj.insert("password",ui->passwdOfUser->text().isEmpty()?"123456":ui->passwdOfUser->text());
        }

        QJsonDocument doc(obj);

        // 锁住用户信息
        lockUserInfo(true);

        // 请求用户管理操作
        postByCurrOperationWithData(0,obj);
    });

    connect(ui->btnOfClassManagement,&QPushButton::clicked,this,[=]()
    {
        // 若未载入班级数据且不为添加班级，则返回
        if(ui->btnOfLoadClass->text()!="取消载入"&&currOperation::getInstance().get()!=operationOfManagement::ADD_CLASS)
        {
            ui->textOfOutput->setText("请先载入班级信息");
            return;
        }

        // 若未输入班级 id、班级信息，则返回
        if(ui->idOfClass->text().isEmpty()||ui->nameOfClass->text().isEmpty()||ui->teacherOfClass->text().isEmpty())
        {
            ui->textOfOutput->setText("未输入班级id与班级名称、任课老师，无法进行提交");
            return;
        }

        QJsonObject obj;
        obj.insert("id",ui->idOfClass->text());
        // 在添加班级、修改班级时使用，在删除班级时不使用以下字段数据
        if(currOperation::getInstance().get()!=operationOfManagement::REMOVE_CLASS)
        {
            obj.insert("name",ui->nameOfClass->text());
            obj.insert("teacherId",ui->teacherOfClass->text());
        }

        lockClassInfo(true);

        // 请求班级管理操作
        postByCurrOperationWithData(1,obj);
    });

    connect(ui->btnOfClassAlloc,&QPushButton::clicked,this,[=]()
    {
        // 未输入用户 id、班级 id，则返回
        if(ui->idOfUserForAlloc->text().isEmpty()||ui->idOfClassForAlloc->text().isEmpty())
        {
            ui->textOfOutput->setText("未输入用户id与班级id，无法进行分配");
            return;
        }

        QJsonObject obj;
        obj.insert("id",ui->idOfClassForAlloc->text());
        obj.insert("userId",ui->idOfUserForAlloc->text());

        // 请求班级管理操作
        postByCurrOperationWithData(1,obj);
    });

    connect(ui->btnOfBatch,&QPushButton::clicked,this,[=]()
    {
        // 未输入为空，则返回
        QString str=ui->inputOfBatch->toPlainText();
        if(str.isEmpty())
        {
            ui->textOfOutput->setText("未输入内容，无法进行批量处理");
            return;
        }

        QJsonObject obj;

        // 批处理操作
        postByCurrOperationWithData(3,obj);
    });

    connect(ui->btnOfLoadUser,&QPushButton::clicked,this,[=]()
    {
        if(ui->btnOfLoadUser->text()=="载入")
        {
            loadUserInfo(true);
        }
        else
        {
            loadUserInfo(false);
        }
    });

    connect(ui->btnOfLoadClass,&QPushButton::clicked,this,[=]()
    {
        if(ui->btnOfLoadClass->text()=="载入")
        {
            loadClassInfo(true);
        }
        else
        {
            loadClassInfo(false);
        }
    });
}

// 载入用户信息，needToLoad 为 true 则载入，needToClearId 为 true 则清空数据
void managementpage::loadUserInfo(bool needToLoad,bool needToClearId)
{
    if(needToClearId)
    {
        ui->idOfUser->clear();
    }

    // 取消载入
    if(!needToLoad)
    {
        ui->btnOfLoadUser->setText("载入");
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

// 载入班级信息，needToLoad 为 true 则载入。needToClearId 为 true 则清空数据
void managementpage::loadClassInfo(bool needToLoad, bool needToClearId)
{
    if(needToClearId)
    {
        ui->idOfClass->clear();
    }

    // 取消载入
    if(!needToLoad)
    {
        ui->btnOfLoadClass->setText("载入");
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

// 锁住用户信息
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

// 锁住班级信息
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

// 清空用户信息
void managementpage::clearUserInfo()
{
    ui->nameOfUser->clear();
    ui->roleOfUser->setCurrentIndex(0);
    ui->passwdOfUser->clear();
}

// 清空班级信息
void managementpage::clearClassInfo()
{
    ui->nameOfClass->clear();
    ui->teacherOfClass->clear();
}

// 执行指定函数，请求指定操作，funcId 为 0-3，分别对于用户管理、班级管理、查询、未分配班级的用户列表的请求操作
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
    case 3:
        connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::operationOfBatch);
        break;
    case 4:
        connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&managementpage::loadUnallocatedStudent);
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

// 执行指定函数，请求当前子操作，funcId 为 0-3，分别对于用户管理、班级管理、查询、未分配班级的用户列表的请求操作
void managementpage::postByCurrOperationWithData(int funcId, const QJsonObject &obj)
{
    postByOperationWithData(funcId,currOperation::getInstance().get(),obj);
}