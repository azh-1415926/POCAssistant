#include "updatepage.h"
#include "ui_updatepage.h"

#include <QJsonObject>
#include <QJsonDocument>

updatepage::updatepage(QWidget *parent)
    : basepage("在线更新",parent)
    , ui(new Ui::updatepage)
{
    initalUpdatePage();
}

updatepage::~updatepage()
{
}

void updatepage::resetPage()
{
    ui->allWidgets->setCurrentIndex(0);
}

void updatepage::back()
{
}

void updatepage::initalUpdatePage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/update");

    connect(ui->allWidgets,&QTabWidget::currentChanged,this,[=](int index)
    {
        if(index==0)
        {
            static_cast<basepage*>(ui->course)->select();
        }
    });

    connect(ui->btnOfEdit,&QPushButton::clicked,this,[=]()
    {
        ui->course->setEditable(true);
    });

    connect(ui->btnOfSubmit,&QPushButton::clicked,this,[=]()
    {
        auto data=ui->course->getCurrCourse();

        QNetworkRequest request;
        request.setUrl(URL_OF_SERVER+"/Course/update");
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");

        QJsonObject obj;
        obj.insert("token",tokenOfAdmin::getInstance().get());
        obj.insert("chapter",data.first.first);
        obj.insert("section",data.first.second);
        obj.insert("content",data.second);
        QJsonDocument doc(obj);

        HTTP_MANAGER->post(request,doc.toJson());
    });

    static_cast<basepage*>(ui->course)->select();
}
