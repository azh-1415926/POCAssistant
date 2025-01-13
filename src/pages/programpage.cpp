#include "programpage.h"
#include "ui_programpage.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>

#include "jsonFile.hpp"

programpage::programpage(QWidget *parent)
    : basepage("在线实验",parent)
    , ui(new Ui::programpage)
{
    initalProgramPage();
}

programpage::~programpage()
{
}

void programpage::resetPage()
{
}

void programpage::selectedPage()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getExperiment);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Code/getExperiment");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    obj.insert("studentId",userId::getInstance().get());
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void programpage::back()
{
}

void programpage::getExperiment(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getExperiment);

    QString str(reply->readAll());
    qDebug()<<"getExperiment:"<<str;

    jsonFile json;
    json.fromJson(str);

    QJsonObject experimentNeedFinish;
    QJsonObject experimentInfo=json.toJson();

    int countOfNeedFinish=0;
    int count=experimentInfo.value("count").toInt();

    ui->optionOfSubmit->clear();
    ui->optionOfSubmit->blockSignals(true);

    for(int i=0;i<count;i++)
    {
        QJsonObject experiment=experimentInfo.value(QString::number(i)).toObject();

        if(experiment.value("isfinish").toInt()==0)
        {
            ui->optionOfSubmit->addItem(experiment.value("name").toString());

            experimentNeedFinish[QString::number(countOfNeedFinish++)]=experiment;
        }
    }

    experimentNeedFinish["count"]=countOfNeedFinish;
    m_ExperimentInfo=experimentNeedFinish;

    ui->optionOfSubmit->blockSignals(false);
}

void programpage::getResult(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getResult);

    QString str(reply->readAll());
    qDebug()<<"compile:"<<str;

    jsonFile json;
    json.fromJson(str);

    ui->infoOfOutput->setText(json.value("output").toString());
}

void programpage::getSubmitState(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getSubmitState);

    QString str(reply->readAll());
    qDebug()<<"submit:"<<str;

    jsonFile json;
    json.fromJson(str);
}

void programpage::initalProgramPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/code");

    setupEditor();

    ui->optionOfSample->addItems(QStringList()<<"hello"<<"sum");

    connect(ui->btnOfRun,&QPushButton::clicked,this,[=]()
    {
        connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getResult);

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

    connect(ui->btnOfSubmit,&QPushButton::clicked,this,[=]()
    {
        connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&programpage::getSubmitState);

        QString code=ui->CodeEdit->toPlainText();

        QString experimentId;

        for(int i=0;i<m_ExperimentInfo.value("count").toInt();i++)
        {
            QJsonObject experiment=m_ExperimentInfo.value(QString::number(i)).toObject();
            if(ui->optionOfSubmit->currentText()==experiment.value("name").toString())
            {
                experimentId=QString::number(experiment.value("id").toInt());
            }
        }

        qDebug()<<"experimentId:"<<experimentId;

        QNetworkRequest request;
        request.setUrl(URL_OF_SERVER+"/Code/submit");
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        QJsonObject obj;
        obj.insert("code",code);
        obj.insert("experimentId",experimentId);
        QJsonDocument doc(obj);

        HTTP_MANAGER->post(request,doc.toJson());
    });
}

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