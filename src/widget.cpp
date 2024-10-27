#include "widget.h"
#include "ui_widget.h"

#include "base.hpp"
#include "jsonFile.hpp"

#define RegisterPage(pageName) \
    connect(pageName,&basepage::refreshStatus,this,&widget::setPageStatus); \
    connect(pageName,&basepage::logon,this,&widget::hideLoginPage); \
    connect(pageName,&basepage::logoff,this,&widget::showLoginPage);

static QString currInfo;

widget::widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget)
    , m_FlagOfInit(false)
{
    initalWidget();

    m_FlagOfInit=true;
}

widget::~widget()
{
}

bool widget::eventFilter(QObject *obj, QEvent *e)
{
    if(obj==this&&e->type()==QEvent::Close)
    {
        if(ui->TopPages->currentIndex()==1)
        {
            e->ignore();

            showLoginPage();
            
            return true;
        }
    }

    return false;
}

void widget::showLoginPage()
{
    ui->TopPages->setCurrentIndex(0);
}

void widget::hideLoginPage()
{
    connect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&widget::getInfo);

    QNetworkRequest request;
    request.setUrl(QUrl("http://127.0.0.1:8848/login/info?userId="+userId::getInstance().get()));
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");
    // request.setRawHeader("Cookie",httpCookie::getInstance().cookie().toUtf8());
    qDebug()<<request.rawHeaderList();

    httpManager::getInstance().get()->get(request);
}

void widget::setPageStatus(const StatusOfPage &status)
{
    ui->CurrPage->setText(status.currPage);
    ui->CurrIcon->setPixmap(status.currIcon);
    if(status.currInfo.isEmpty())
        ui->CurrInfo->setText(currInfo);
    else
        ui->CurrInfo->setText(status.currInfo);
}

void widget::goToBack()
{
    QWidget* page=ui->AllPages->currentWidget();
    static_cast<basepage*>(page)->back();
}

void widget::selectPage(int i)
{
    QWidget* page=ui->AllPages->currentWidget();
    static_cast<basepage*>(page)->resetPage();
    ui->AllPages->setCurrentIndex(i);

    ui->AllPages->setCurrentIndex(i);
    page=ui->AllPages->currentWidget();
    static_cast<basepage*>(page)->select();
}

void widget::getInfo(QNetworkReply *reply)
{
    disconnect(httpManager::getInstance().get(), &QNetworkAccessManager::finished,this,&widget::getInfo);

    QString str(reply->readAll());
    qDebug()<<"getInfo:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString prefix="当前用户：";
    QString suffix;
    int role=json.value("role").toInt();

    switch (role)
    {
    case 0:
        suffix="同学";
        break;
    
    case 1:
        suffix="老师";
        break;

    case 2:
        suffix="管理员";
        break;

    default:
        break;
    }

    if(!json.value("user_name").toString().isEmpty())
        currInfo=prefix+json.value("user_name").toString()+suffix;

    qDebug()<<"currInfo"<<currInfo;

    ui->TopPages->setCurrentIndex(1);
    ui->MainPage->select();
}

void widget::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    // ui->frame->setStyleSheet("QFrame#frame{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(251,102,102, 200), stop:1 rgba(20,196,188, 210));}");
    ui->frame->setStyleSheet("QFrame#frame{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,255,210, 200), stop:1 rgba(168, 216, 234, 210));}");

    initalTopPages();
    initalAllPages();

    showLoginPage();
}

void widget::initalTopPages()
{
    connect(ui->LoginPage,&basepage::logon,this,&widget::hideLoginPage);
    connect(ui->LoginPage,&basepage::logoff,this,&widget::showLoginPage); 
}

void widget::initalAllPages()
{
    connect(ui->HomeIcon,&clicklabel::clicked,this,[=]()
    {
        selectPage(0);
    });
    connect(ui->BackIcon,&clicklabel::clicked,this,&widget::goToBack);

    initalMainPage();
}

void widget::initalMainPage()
{
    RegisterPage(ui->MainPage)
    RegisterPage(ui->CoursePage)
    RegisterPage(ui->ProgramPage)
    RegisterPage(ui->QuizPage)

    connect(ui->MainPage,&mainpage::selectPage,this,&widget::selectPage);

    selectPage(0);
}