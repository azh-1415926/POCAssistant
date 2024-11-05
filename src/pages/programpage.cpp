#include "programpage.h"
#include "ui_programpage.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>

#include "jsonFile.hpp"
#include "constant.h"

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

void programpage::back()
{
}

void programpage::getResult(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&programpage::getResult);

    QString str(reply->readAll());
    qDebug()<<"compile:"<<str;

    jsonFile json;
    json.fromJson(str);

    ui->infoOfOutput->setText(json.value("output").toString());
}

void programpage::initalProgramPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/code");

    setupEditor();

    ui->optionOfSample->addItems(QStringList()<<"hello"<<"sum");

    connect(ui->btnOfRun,&QPushButton::clicked,this,[=]()
    {
        connect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&programpage::getResult);

        QString code=ui->CodeEdit->toPlainText();

        QNetworkRequest request;
        request.setUrl(QUrl("http://"
        SERVER_IP
        ":"
        SERVER_PORT_S
        "/code/compile"));
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        QJsonObject obj;
        obj.insert("data",code);
        QJsonDocument doc(obj);

        httpManager::getInstance().get()->post(request,doc.toJson());
    });
}

void programpage::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->CodeEdit->setFont(font);

    highlighter = new Highlighter(ui->CodeEdit->document());

    connect(ui->optionOfSample,&QComboBox::currentTextChanged,this,[=](const QString& sample)
    {
        QFile file(":/sample-code/"+sample+".cpp");
        if (file.open(QFile::ReadOnly | QFile::Text))
            ui->CodeEdit->setPlainText(file.readAll());
    });
}