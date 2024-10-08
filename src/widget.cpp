#include "widget.h"
#include "ui_widget.h"

#define RegisterPage(pageName) \
    connect(pageName,&basepage::refreshStatus,this,&widget::setPageStatus); \
    connect(pageName,&basepage::logon,this,&widget::hideLoginPage); \
    connect(pageName,&basepage::logoff,this,&widget::showLoginPage);

// #define 

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
    ui->TopPages->setCurrentIndex(1);
}

void widget::setPageStatus(const StatusOfPage &status)
{
    ui->CurrPage->setText(status.currPage);
    ui->CurrIcon->setPixmap(status.currPage);
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
    connect(ui->LoginPage,&basepage::logon,this,[=](const QString& token)
    {
        qDebug()<<"Token:"<<token;

        hideLoginPage();
    });
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