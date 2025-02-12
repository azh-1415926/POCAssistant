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
    else if(userRole::getInstance().get()==(int)UserRole::TEACHER)
    {
        ui->stackedWidget->setCurrentIndex(1);

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

    ui->infoOfOutput->setText(json.value("output").toString());
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
}

void programpage::initalProgramPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/code");

    setupEditor();

    ui->optionOfSample->addItems(QStringList()<<"hello"<<"sum");

    connect(ui->btnOfRun,&QPushButton::clicked,this,[=]()
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
    });

    // 学生提交实验的逻辑
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

        azh::logger()<<"programpage experimentId:"<<experimentId;

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
    });

    // 教师发布实验的逻辑
    connect(ui->btnOfRelease,&QPushButton::clicked,this,[=]()
    {
        int index=ui->optionOfClass->currentIndex();

        if(index==-1)
        {
            ui->outputOfRelease->setText("未选择班级");
            return;
        }

        QString name=ui->inputOfProgramTitle->text();
        QString content=ui->inputOfProgramContent->toPlainText();

        if(name.isEmpty()||content.isEmpty())
        {
            ui->outputOfRelease->setText("请输入实验名称以及实验内容以便发布实验");
            return;
        }

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
    });

    // 更新学生的实验内容
    connect(ui->optionOfProgram,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        if(i==-1)
        {
            return;
        }

        QJsonObject experiment=m_UnFinishedExperimentInfo.value(QString::number(i)).toObject();
        QString content=experiment.value("content").toString();

        ui->contentOfProgram->setText(content);
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