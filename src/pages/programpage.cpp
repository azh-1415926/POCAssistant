#include "programpage.h"
#include "ui_programpage.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
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

void programpage::resetPage()
{
}

void programpage::back()
{
}

void programpage::initalProgramPage()
{
    ui->setupUi(this);

    setupEditor();

    ui->optionOfSample->addItems(QStringList()<<"hello"<<"sum");

    connect(ui->btnOfRun,&QPushButton::clicked,this,[=]()
    {
        QString code=ui->CodeEdit->toPlainText();

        QNetworkRequest request;
        request.setUrl(QUrl("http://127.0.0.1:8848/code/compile"));
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        QJsonObject obj;
        obj.insert("data",code);
        QJsonDocument doc(obj);

        httpManager->post(request,doc.toJson());
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