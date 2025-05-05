#include "quizpage.h"
#include "ui_quizpage.h"

#include "quizcontroller.h"
#include "quizmodel.h"

#include <QComboBox>

// 中文数字文本转 int
inline QString number2bigChinese(int i)
{
    QStringList numerical_value  = {"零","一","二","三","四","五","六","七","八","九"};

    return i<10?numerical_value[i]:numerical_value[0];
}

quizpage::quizpage(QWidget *parent)
    : basepage("在线测试",parent)
    , ui(new Ui::quizpage)
{
    initalQuizPage();
}

quizpage::~quizpage()
{
}

void quizpage::resetPage()
{
    if(userRole::getInstance().get()==(int)UserRole::STUDENT)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(userRole::getInstance().get()==(int)UserRole::TEACHER)
    {
        ui->stackedWidget->setCurrentIndex(4);

        requestClass();
    }
}

void quizpage::selectedPage()
{
    resetPage();
}

void quizpage::back()
{
    int currIndex=ui->stackedWidget->currentIndex();

    // 若为学生模块，重置到下标 0，教师页面下标为 4，且暂无子页面
    if(currIndex>=1&&currIndex<4)
        ui->stackedWidget->setCurrentIndex(0);
}

void quizpage::updateChapter(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::updateChapter);

    QString str(reply->readAll());
    azh::logger()<<"quizpage updatechapter:"<<str;

    jsonFile json;
    json.fromJson(str);

    int chapter=json.value("chapter").toInt();

    ui->optionOfChapter->clear();

    for(int i=0;i<chapter;i++)
    {
        ui->optionOfChapter->addItem("第"+number2bigChinese(i+1)+"章");
    }
}

void quizpage::updateTest(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::updateTest);

    QString str(reply->readAll());
    azh::logger()<<"quizpage updatetest:"<<str;

    jsonFile json;
    json.fromJson(str);

    modelOfTest->load(json.toJson());
}

void quizpage::updateCollection(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::updateCollection);

    QString str(reply->readAll());
    azh::logger()<<"quizpage updatecollection:"<<str;

    jsonFile json;
    json.fromJson(str);

    modelOfCollectedTest->load(json.toJson());
}

void quizpage::updateWrong(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::updateWrong);
}

void quizpage::getCollectedStatus(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getCollectedStatus);

    updateUncollectedStatus();
}

void quizpage::getUncollectedStatus(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getUncollectedStatus);

    // requestChapter();
    switchQuizToTestOrCollection();
}

void quizpage::getAnswer(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getAnswer);

    QString str(reply->readAll());
    azh::logger()<<"quizpage getanswer:"<<str;

    jsonFile json;
    json.fromJson(str);

    int count=json.value("count").toInt();

    QStringList listOfUserAnswer=modelOfTest->getAnswer();
    QJsonArray listOfWrong;

    for(int i=0;i<count;i++)
    {
        QJsonObject quiz=json.value(QString::number(i)).toObject();

        if(listOfUserAnswer[i]==quiz.value("answer").toString())
        {
            listOfWrong.append(i);
        }
    }

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Quiz/addWrongQuiz");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("studentId",userId::getInstance().get());
    obj.insert("data",listOfWrong);
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::getClass(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getClass);

    QString str(reply->readAll());
    azh::logger()<<"quizpage getclass:"<<str;

    jsonFile json;
    json.fromJson(str);

    int count=json.value("count").toInt();

    classList.clear();
    QStringList listOfClassName;

    for(int i=0;i<count;i++)
    {
        QJsonObject info=json.value(QString::number(i)).toObject();

        QString name=info.value("name").toString();

        classList<<QPair<QString,QString>(info.value("id").toString(),name);
        listOfClassName<<name;
    }
    
    ui->optionOfClass->clear();
    ui->optionOfClass->addItems(listOfClassName);
}

void quizpage::getStudent(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getStudent);

    QString str(reply->readAll());
    azh::logger()<<"quizpage getstudent:"<<str;

    jsonFile json;
    json.fromJson(str);

    int count=json.value("count").toInt();

    studentInfo.clear();
    QStringList listOfStudentName;
    
    for(int i=0;i<count;i++)
    {
        QJsonObject info=json.value(QString::number(i)).toObject();

        QString name=info.value("name").toString();
        studentInfo<<QPair<QString,QString>(info.value("id").toString(),name);

        listOfStudentName<<name;
    }

    ui->optionOfStudent->clear();
    ui->optionOfStudent->addItems(listOfStudentName);
}

void quizpage::getExperiment(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getExperiment);

    QString str(reply->readAll());
    azh::logger()<<"quizpage getexperiment:"<<str;

    jsonFile json;
    json.fromJson(str);

    int count=json.value("count").toInt();

    experimentInfo.clear();
    experimentContent.clear();
    QStringList listOfExperimentName;
    
    for(int i=0;i<count;i++)
    {
        QJsonObject info=json.value(QString::number(i)).toObject();

        QString name=info.value("name").toString();
        experimentInfo<<QPair<QString,QString>(info.value("id").toString(),name);
        experimentContent<<QPair<QString,QString>(info.value("content").toString(),info.value("code").toString());

        listOfExperimentName<<name;
    }

    ui->optionOfExperiment->clear();
    ui->optionOfExperiment->addItems(listOfExperimentName);
}

void quizpage::getSubmitScoreStatus(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getSubmitScoreStatus);

    QString str(reply->readAll());
    azh::logger()<<"quizpage getsubmitscorestatus:"<<str;

    jsonFile json;
    json.fromJson(str);
    
    ui->outputOfSubmitScore->setText(json.value("info").toString());

    QString result=json.value("result").toString();
    if(!result.isEmpty())
    {
        // 若为提交成功，刷新实验框
        if(result=="true")
        {
            requestExperiment(studentInfo.at(ui->optionOfStudent->currentIndex()).first);
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
}

void quizpage::initalQuizPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/quiz");

    // 初始化章节测试中答题页和收藏页
    quizcontroller* controllerOfTest=new quizcontroller(this);
    quizcontroller* controllerOfCollectedTest=new quizcontroller(this);

    modelOfTest=new quizmodel(this);
    modelOfCollectedTest=new quizmodel(this);

    controllerOfTest->bind(ui->pageOfQuestions);
    controllerOfTest->bind(modelOfTest);

    controllerOfCollectedTest->bind(ui->pageOfCollections);
    controllerOfCollectedTest->bind(modelOfCollectedTest);

    // 初始化错题回顾
    quizcontroller* controllerOfWrong=new quizcontroller(this);
    modelOfWrongQuiz=new quizmodel(this);
    controllerOfWrong->bind(ui->wrongQuiz);
    controllerOfWrong->bind(modelOfWrongQuiz);
    ui->wrongQuiz->setReadOnly(true);
    ui->wrongQuiz->hideCollection(true);

    jsonFile file;
    file.load(":/json/test/quiz.json");
    modelOfWrongQuiz->load(file.toJson());
    
    ui->wrongQuiz->setAnswerIndex(1);
    ui->wrongQuiz->setOption(0);

    QRegularExpressionValidator* reg = new QRegularExpressionValidator(this);
    // 正则匹配两位数字，或 100
    reg->setRegularExpression(QRegularExpression(QString("\\d{1,2}|100")));
    ui->inputOfScore->setValidator(reg);

    // 章节测试按钮
    connect(ui->btnOfTest,&QPushButton::clicked,this,[=]()
    {
        ui->stackedWidget->setCurrentIndex(1);

        requestChapter();
    });

    connect(ui->optionOfChapter,&QComboBox::currentIndexChanged,this,[=]()
    {
        switchQuizToTestOrCollection(false);
    });

    // 错题回顾按钮
    connect(ui->btnOfWrong,&QPushButton::clicked,this,[=]()
    {
        ui->stackedWidget->setCurrentIndex(2);
    });

    // 切换答题页与收藏页的按钮
    connect(ui->btnOfSwitch,&QPushButton::clicked,this,[=]()
    {
        updateCollectedStatus();
    });

    // 提交答题页的按钮
    connect(ui->btnOfSubmitTest,&QPushButton::clicked,this,[=]()
    {
        requestAnswer();
    });

    // 教师批改的下拉框
    connect(ui->optionOfClass,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        if(i==-1)
            return;
        
        requestStudent(classList.at(i).first);
    });

    connect(ui->optionOfStudent,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        if(i==-1)
            return;

        requestExperiment(studentInfo.at(i).first);
    });

    connect(ui->optionOfExperiment,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        if(i==-1)
        {
            ui->contentOfQuiz->setText("暂无实验");
            ui->contentOfAnswer->setText("实验已批改完成，或未提交实验");
            return;
        }

        ui->contentOfQuiz->setText(experimentContent.at(i).first);
        ui->contentOfAnswer->setText(experimentContent.at(i).second);
    });

    // 教师提交批改分数的按钮
    connect(ui->btnOfSubmitScore,&QPushButton::clicked,this,[=]()
    {
        submitScore(studentInfo.at(ui->optionOfStudent->currentIndex()).first,experimentInfo.at(ui->optionOfExperiment->currentIndex()).first,ui->inputOfScore->text().toInt());
    });

    ui->frame3->hide();
}

void quizpage::requestChapter()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::updateChapter);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Quiz/getChapter");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::requestTestQuiz()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::updateTest);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Quiz/getQuiz");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("chapter",ui->optionOfChapter->currentIndex()+1);
    obj.insert("studentId",userId::getInstance().get());
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::requestCollectedQuiz()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::updateCollection);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Quiz/getCollectedQuiz");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("chapter",ui->optionOfChapter->currentIndex()+1);
    obj.insert("studentId",userId::getInstance().get());
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::requestAnswer()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getAnswer);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Quiz/getAnswer");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("chapter",ui->optionOfChapter->currentIndex()+1);
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::switchQuizToTestOrCollection(bool switchToNext)
{
    // 是否为答题页
    static bool isTest=true;

    if(!switchToNext)
    {
        if(isTest)
        {
            requestTestQuiz();
        }
        else
        {
            requestCollectedQuiz();
        }
        
        return;
    }
        
    if(isTest)
    {
        // 当前应切换到收藏页
        requestCollectedQuiz();

        ui->btnOfSwitch->setText("切换至答题页");
        ui->pagesOfTest->setCurrentIndex(1);

        ui->btnOfSubmitTest->setEnabled(false);
        isTest=false;
    }
    else
    {
        // 当前应切换到答题页
        requestTestQuiz();

        if(switchToNext)
        {
            ui->btnOfSwitch->setText("切换至收藏页");
            ui->pagesOfTest->setCurrentIndex(0);

            ui->btnOfSubmitTest->setEnabled(true);
            isTest=true;
        }
    }
}

void quizpage::requestClass()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getClass);

    QNetworkRequest request;
    
    request.setUrl(URL_OF_SERVER+"/Class/getClassByTeacher");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    
    obj.insert("id",userId::getInstance().get());
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::requestStudent(const QString& classId)
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getStudent);

    QNetworkRequest request;
    
    request.setUrl(URL_OF_SERVER+"/Class/getStudentByClass");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    
    obj.insert("id",classId);
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::requestExperiment(const QString &studentId)
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getExperiment);

    QNetworkRequest request;
    
    request.setUrl(URL_OF_SERVER+"/Code/getExperimentByStudent");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    
    obj.insert("id",studentId);
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::updateCollectedStatus()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getCollectedStatus);

    QNetworkRequest request;
    
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    
    if(ui->btnOfSwitch->text()=="切换至收藏页")
    {
        obj=modelOfTest->getCollectedIndex();
    }
    else
    {
        obj=modelOfCollectedTest->getCollectedIndex();
    }
    
    obj.insert("chapter",ui->optionOfChapter->currentIndex()+1);
    obj.insert("studentId",userId::getInstance().get());
    QJsonDocument doc(obj);

    request.setUrl(URL_OF_SERVER+"/Quiz/collectQuiz");
    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::updateUncollectedStatus()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getUncollectedStatus);

    QNetworkRequest request;
    
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    
    if(ui->btnOfSwitch->text()=="切换至收藏页")
    {
        obj=modelOfTest->getUncollectedIndex();
    }
    else
    {
        obj=modelOfCollectedTest->getUncollectedIndex();
    }
    
    obj.insert("chapter",ui->optionOfChapter->currentIndex()+1);
    obj.insert("studentId",userId::getInstance().get());
    QJsonDocument doc(obj);

    request.setUrl(URL_OF_SERVER+"/Quiz/uncollectQuiz");
    HTTP_MANAGER->post(request,doc.toJson());
}

void quizpage::submitScore(const QString &studentId,const QString &experimentId, int score)
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&quizpage::getSubmitScoreStatus);

    QNetworkRequest request;
    
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    
    obj.insert("studentId",studentId);
    obj.insert("experimentId",experimentId);
    obj.insert("score",score);
    QJsonDocument doc(obj);

    request.setUrl(URL_OF_SERVER+"/Code/submitScore");
    HTTP_MANAGER->post(request,doc.toJson());
}
