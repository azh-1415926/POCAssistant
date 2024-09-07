#include "widget.h"
#include "ui_widget.h"

widget::widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget)
    , m_FlagOfInit(false)
{
    initalWidget();
    initalMainPage();

    m_FlagOfInit=true;
}

widget::~widget()
{
}

void widget::setPageStatus(const StatusOfPage& status)
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

void widget::selectPage(const clicklabel *label)
{
    static QList<clicklabel*> list=
    {
        ui->course,ui->program,ui->question
    };

    int i=list.indexOf(label);
    if(i==-1)
        return;

    selectPage(i+1);
}

void widget::initalWidget()
{
    ui->setupUi(this);

    connect(ui->HomeIcon,&clicklabel::clicked,this,[=]()
    {
        selectPage(0);
    });
    connect(ui->BackIcon,&clicklabel::clicked,this,&widget::goToBack);
}

void widget::initalMainPage()
{
    ui->MainPage->setStatus("主页","");
    connect(ui->MainPage,&basepage::refreshStatus,this,&widget::setPageStatus);

    connect(ui->CoursePage,&basepage::refreshStatus,this,&widget::setPageStatus);
    connect(ui->ProgramPage,&basepage::refreshStatus,this,&widget::setPageStatus);
    connect(ui->QuestionPage,&basepage::refreshStatus,this,&widget::setPageStatus);

    connect(ui->course,&clicklabel::clicked,this,QOverload<const clicklabel*>::of(&widget::selectPage));
    connect(ui->program,&clicklabel::clicked,this,QOverload<const clicklabel*>::of(&widget::selectPage));
    connect(ui->question,&clicklabel::clicked,this,QOverload<const clicklabel*>::of(&widget::selectPage));

    selectPage(0);
}