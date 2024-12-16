#include "mainpage.h"
#include "ui_mainpage.h"

mainpage::mainpage(QWidget *parent)
    : basepage("主页",parent)
    , ui(new Ui::mainpage)
{
    initalmainpage();
}

mainpage::~mainpage()
{
}

void mainpage::resetPage()
{
}

void mainpage::back()
{
}

void mainpage::selectLabel(const clicklabel* label)
{
    if(label==ui->course)
    {
        emit selectPage(1);
    }
    // program(management)
    else if(label==ui->program)
    {
        // 若为管理员，则跳转至在线管理
        if(userRole::getInstance().get()==(int)(UserRole::ADMIN))
        {
            emit selectPage(4);
            return;
        }

        emit selectPage(2);
    }
    else if(label==ui->quiz)
    {
        emit selectPage(3);
    }
    else
    {
        ;
    }
}

void mainpage::switchToAdmin(bool state)
{
    if(state)
    {
        ui->iconOfProgram->setPixmap(QPixmap(":/img/main/management"));
        ui->program->setText("在线管理");
        ui->textOfProgram->setText("<html><head/><body><p>提供在线管理功能，只能由管理员对学生、教师、班级信息进行增删改查</p></body></html>");
    }
    else
    {
        ui->iconOfProgram->setPixmap(QPixmap(":/img/main/code"));
        ui->program->setText("在线实验");
        ui->textOfProgram->setText("<html><head/><body><p>提供在线代码编辑器，支持C语言，用户可以在线编写代码并进行编译，查看编译结果</p></body></html>");
    }
}

void mainpage::initalmainpage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/home");

    connect(ui->course,&clicklabel::clicked,this,&mainpage::selectLabel);
    connect(ui->program,&clicklabel::clicked,this,&mainpage::selectLabel);
    connect(ui->quiz,&clicklabel::clicked,this,&mainpage::selectLabel);

    connect(ui->btnOfCourse,&QPushButton::clicked,this,[=](){ emit selectPage(1); });
    
    connect(ui->btnOfProgram,&QPushButton::clicked,this,[=]()
    {
        if(userRole::getInstance().get()==(int)(UserRole::ADMIN))
            emit selectPage(4);
        else
            emit selectPage(2);
    });
    connect(ui->btnOfQuiz,&QPushButton::clicked,this,[=](){ emit selectPage(3); });
}
