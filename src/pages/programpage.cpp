#include "programpage.h"
#include "ui_programpage.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>

programpage::programpage(QWidget *parent)
    : basepage("在线实验",parent)
    , ui(new Ui::programpage)
{
    initalProgramPage();
}

programpage::~programpage()
{
}

// 重置页面的时候，切换到学生、教师页面
void programpage::resetPage()
{
    if(userRole::getInstance().get()==(int)UserRole::STUDENT)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(userRole::getInstance().get()==(int)UserRole::TEACHER)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

// 初始化展示该页面的时候，对于学生，更新当前所需完成的实验列表，对于教师，更新当前管理的班级列表
void programpage::selectedPage()
{
    if(userRole::getInstance().get()==(int)UserRole::STUDENT)
    {
        ui->stackedWidget->setCurrentIndex(0);

        requestUnfinishedExperiment();
    }
    else if(userRole::getInstance().get()==(int)UserRole::TEACHER)
    {
        ui->stackedWidget->setCurrentIndex(1);

        requestClassInfo();
    }
}

void programpage::back()
{
}

// 接收后端数据，更新学生当前未完成的实验列表
void programpage::getUnfinishedExperiment(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getUnfinishedExperiment);

    QString str(reply->readAll());
    azh::logger()<<"programpage getUnfinishedExperiment:"<<str;

    jsonFile json;
    json.fromJson(str);

    QJsonObject experimentNeedFinish;
    QJsonObject experimentInfo=json.toJson();

    // 逐一提取出实验信息
    int countOfNeedFinish=0;
    int count=experimentInfo.value("count").toInt();

    ui->optionOfProgram->clear();

    for(int i=0;i<count;i++)
    {
        QJsonObject experiment=experimentInfo.value(QString::number(i)).toObject();

        ui->optionOfProgram->addItem(experiment.value("name").toString());

        experimentNeedFinish[QString::number(countOfNeedFinish++)]=experiment;
    }

    experimentNeedFinish["count"]=countOfNeedFinish;
    m_UnFinishedExperimentInfo=experimentNeedFinish;

    if(countOfNeedFinish==0)
    {
        ui->btnOfSubmit->setEnabled(false);
    }
    else
    {
        ui->btnOfSubmit->setEnabled(true);

        QJsonObject experiment=m_UnFinishedExperimentInfo.value(QString::number(0)).toObject();
        QString content=experiment.value("content").toString();

        ui->contentOfProgram->setText(content);
    }
}

// 接收后端数据，得到学生编译结果
void programpage::getCompileOutput(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getCompileOutput);

    QString str(reply->readAll());
    azh::logger()<<"programpage compile:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        // 若为提交成功，设置输出文本
        if(result=="true")
        {
            ui->infoOfOutput->setText(json.value("output").toString());
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
}

// 接收后端数据，得到学生实验提交的结果
void programpage::getSubmitState(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getSubmitState);

    QString str(reply->readAll());
    azh::logger()<<"programpage submit:"<<str;

    jsonFile json;
    json.fromJson(str);

    ui->infoOfOutput->setText(json.value("info").toString());

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        // 若为提交成功，刷新实验框
        if(result=="true")
        {
            requestUnfinishedExperiment();
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
}

// 接收后端数据，更新教师所管理的班级列表
void programpage::getClassInfo(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getClassInfo);

    QString str(reply->readAll());
    azh::logger()<<"programpage getClassInfo:"<<str;

    jsonFile json;
    json.fromJson(str);

    QJsonObject classInfo=json.toJson();

    int count=classInfo.value("count").toInt();

    m_ClassInfo.clear();

    for(int i=0;i<count;i++)
    {
        QJsonObject info=classInfo.value(QString::number(i)).toObject();

        ui->optionOfClass->addItem(info.value("name").toString());

        m_ClassInfo.push_back(QPair<QString,QString>(info.value("id").toString(),info.value("name").toString()));
    }
}

// 接收后端数据，得到教师发布实验的结果
void programpage::getReleaseState(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getReleaseState);

    QString str(reply->readAll());
    azh::logger()<<"programpage release:"<<str;

    jsonFile json;
    json.fromJson(str);

    ui->outputOfRelease->setText(json.value("info").toString());

    ui->inputOfProgramTitle->clear();
    ui->inputOfProgramContent->clear();

    QString result=json.value("result").toString();
    if(!result.isEmpty())
    {
        // 若为提交成功，设置输出文本
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

void programpage::getExperimentByStudent(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getExperimentByStudent);

    QString str(reply->readAll());
    azh::logger()<<"programpage getAllExperiment:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {
        if(result=="true")
        {
            int count=json.value("count").toInt();
            QString info="实验记录ID,实验ID,实验名称,提交时间,分数,是否已批改\n";

            for(int i=0;i<count;i++)
            {
                QJsonObject row=json.value(QString::number(i)).toObject();
                info+=QString::number(row.value("id").toInt())+","
                    +row.value("experimentId").toString()+","
                    +row.value("name").toString()+","
                    +row.value("finishTime").toString()+","
                    +QString::number(row.value("score").toInt())+","
                    +(row.value("isCorrect").toInt()==0?"否":"是")+"\n";
            }

            ui->infoOfOutput->setText(info);
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
}

void programpage::getExperimentByTeacher(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getExperimentByTeacher);

    QString str(reply->readAll());
    azh::logger()<<"programpage getExperimentByTeacher:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    ui->recordOfExperiment->clear();
    
    if(!result.isEmpty())
    {
        if(result=="true")
        {
            int count=json.value("count").toInt();

            QStringList title=
            {
                "学生ID","实验记录ID","实验ID","实验名称","创建时间","提交时间","分数","是否已批改"
            };

            ui->recordOfExperiment->setRowCount(count);
            ui->recordOfExperiment->setColumnCount(title.size());
            ui->recordOfExperiment->setHorizontalHeaderLabels(title);

            for(int i=0;i<count;i++)
            {
                QJsonObject row=json.value(QString::number(i)).toObject();

                ui->recordOfExperiment->setItem(i,0,new QTableWidgetItem(row.value("studentId").toString()));
                ui->recordOfExperiment->setItem(i,1,new QTableWidgetItem(QString::number(row.value("id").toInt())));
                ui->recordOfExperiment->setItem(i,2,new QTableWidgetItem(row.value("experimentId").toString()));
                ui->recordOfExperiment->setItem(i,3,new QTableWidgetItem(row.value("name").toString()));
                ui->recordOfExperiment->setItem(i,4,new QTableWidgetItem(row.value("createTime").toString()));
                ui->recordOfExperiment->setItem(i,5,new QTableWidgetItem(row.value("finishTime").toString()));
                ui->recordOfExperiment->setItem(i,6,new QTableWidgetItem(QString::number(row.value("score").toInt())));
                ui->recordOfExperiment->setItem(i,7,new QTableWidgetItem(row.value("isCorrect").toInt()==0?"否":"是"));
            }
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
}

void programpage::initalProgramPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/code");

    setupEditor();

    // 初始化示例代码（学生）
    ui->optionOfSample->addItems(QStringList()<<"hello"<<"sum");

    // 编译运行代码（学生）
    connect(ui->btnOfRun,&QPushButton::clicked,this,[=]()
    {
        requestCompile();
    });

    // 提交实验（学生）
    connect(ui->btnOfSubmit,&QPushButton::clicked,this,[=]()
    {
        int index=ui->optionOfProgram->currentIndex();

        if(index==-1)
        {
            return;
        }

        QString code=ui->CodeEdit->toPlainText();

        QJsonObject experiment=m_UnFinishedExperimentInfo.value(QString::number(index)).toObject();
        QString experimentId=experiment.value("experimentId").toString();

        if(experimentId.isEmpty())
        {
            ui->infoOfOutput->setText("提交实验失败，未选择任何实验");
            return;
        }

        requestSubmitExperiment();
    });

    // 更新实验内容（学生）
    connect(ui->optionOfProgram,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        if(i==-1)
        {
            ui->contentOfProgram->setText("暂无实验");
            return;
        }

        QJsonObject experiment=m_UnFinishedExperimentInfo.value(QString::number(i)).toObject();
        QString content=experiment.value("content").toString();

        ui->contentOfProgram->setText(content);
    });

    // 发布实验（教师）
    connect(ui->btnOfRelease,&QPushButton::clicked,this,[=]()
    {
        if(ui->optionOfClass->currentIndex()==-1)
        {
            ui->outputOfRelease->setText("未选择班级");
            return;
        }

        if(ui->inputOfProgramTitle->text().isEmpty()||ui->inputOfProgramContent->toPlainText().isEmpty())
        {
            ui->outputOfRelease->setText("请输入实验名称以及实验内容以便发布实验");
            return;
        }

        requestReleaseExperiment();
    });

    // 发布实验页面中按钮，用于切换至查看实验记录页面
    connect(ui->btnOfTeacher,&QPushButton::clicked,this,[=]()
    {
        // 切换至教师的查看实验记录
        ui->stackedWidget->setCurrentIndex(2);
        // 请求实验记录
        requestExperimentRecord();
    });

    //查看实验记录页面中按钮，用于切换至发布实验页面
    connect(ui->btnOfTeacher2,&QPushButton::clicked,this,[=]()
    {
        // 切换至教师的发布实验
        ui->stackedWidget->setCurrentIndex(1);
    });

    //查询全部实验记录（学生、教师）
    connect(ui->btnOfSelectAll,&QPushButton::clicked,this,[=]()
    {
        requestExperimentRecord();
    });
}

// 设置代码输入框的语法高亮
void programpage::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->CodeEdit->setFont(font);
    ui->CodeEdit->setAcceptRichText(false);

    highlighter = new Highlighter(ui->CodeEdit->document());

    connect(ui->optionOfSample,&QComboBox::currentTextChanged,this,[=](const QString& sample)
    {
        QFile file(":/sample-code/"+sample+".cpp");
        if (file.open(QFile::ReadOnly | QFile::Text))
            ui->CodeEdit->setPlainText(file.readAll());
    });
}

void programpage::requestUnfinishedExperiment()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getUnfinishedExperiment);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Code/getUnfinishedExperiment");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("studentId",userId::getInstance().get());
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void programpage::requestCompile()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getCompileOutput);

    QString code=ui->CodeEdit->toPlainText();
    QString input=ui->inputOfProgram->toPlainText();

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Code/compile");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("code",code);
    obj.insert("input",input);
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void programpage::requestSubmitExperiment()
{
    int index=ui->optionOfProgram->currentIndex();
    QString code=ui->CodeEdit->toPlainText();
    QJsonObject experiment=m_UnFinishedExperimentInfo.value(QString::number(index)).toObject();
    QString experimentId=experiment.value("experimentId").toString();

    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getSubmitState);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Code/submit");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("code",code);
    obj.insert("studentId",userId::getInstance().get());
    obj.insert("experimentId",experimentId);
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void programpage::requestClassInfo()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getClassInfo);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Class/getClassByTeacher");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("id",userId::getInstance().get());
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void programpage::requestExperimentRecord()
{
    switch (userRole::getInstance().get())
    {
    case 0:
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getExperimentByStudent);
        break;
    
    case 1:
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getExperimentByTeacher);
        break;
    
    default:
        break;
    }

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Code/getExperiment");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("userId",userId::getInstance().get());
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void programpage::requestReleaseExperiment()
{
    int index=ui->optionOfClass->currentIndex();
    QString name=ui->inputOfProgramTitle->text();
    QString content=ui->inputOfProgramContent->toPlainText();

    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getReleaseState);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Code/releaseExperiment");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("classId",m_ClassInfo.at(index).first);
    obj.insert("name",name);
    obj.insert("content",content);
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}
