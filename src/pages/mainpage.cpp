#include "mainpage.h"
#include "ui_mainpage.h"

mainpage::mainpage(QWidget *parent)
    : basepage("主页",parent)
    , ui(new Ui::mainpage)
{
    initalMainPage();
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
    // course(management)
    if(label==ui->course)
    {
        // 若为管理员，则跳转至在线管理
        if(userRole::getInstance().get()==(int)(UserRole::ADMIN))
        {
            emit selectPage(4);
            return;
        }

        emit selectPage(1);
    }
    // program
    else if(label==ui->program)
    {
        // 若为管理员，则跳转至在线统计
        if(userRole::getInstance().get()==(int)(UserRole::ADMIN))
        {
            emit selectPage(5);
            return;
        }

        emit selectPage(2);
    }
    else if(label==ui->quiz)
    {
        // 若为管理员，则跳转至在线更新
        if(userRole::getInstance().get()==(int)(UserRole::ADMIN))
        {
            emit selectPage(6);
            return;
        }

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
        ui->iconOfCourse->setPixmap(QPixmap(":/img/main/management"));
        ui->course->setText("在线管理");
        ui->textOfCourse->setText("<html><head/><body><p>提供在线管理功能，管理员管理学生、教师账号，创建班级，分配教师、学生</p></body></html>");

        ui->iconOfProgram->setPixmap(QPixmap(":/img/main/statement"));
        ui->program->setText("在线统计");
        ui->textOfProgram->setText("<html><head/><body><p>提供在线统计功能，管理员可统计所有用户的学习情况、实验完成情况、答题情况</p></body></html>");

        ui->iconOfQuiz->setPixmap(QPixmap(":/img/main/update"));
        ui->quiz->setText("在线更新");
        ui->textOfQuiz->setText("<html><head/><body><p>提供在线管理功能，管理员负责对编译原理课程大纲进行更新，对题目进行更新</p></body></html>");
    }
    else
    {
        ui->iconOfCourse->setPixmap(QPixmap(":/img/main/course"));
        ui->course->setText("在线课程");
        ui->textOfCourse->setText("<html><head/><body><p>提供编译原理的学习路线，涵盖编译原理的各个方面，如词法分析、语法分析、语义分析等</p></body></html>");

        ui->iconOfProgram->setPixmap(QPixmap(":/img/main/code"));
        ui->program->setText("在线实验");
        ui->textOfProgram->setText("<html><head/><body><p>提供在线代码编辑器，支持C语言，用户可以在线编写代码并进行编译，查看编译结果</p></body></html>");

        ui->iconOfQuiz->setPixmap(QPixmap(":/img/main/quiz"));
        ui->quiz->setText("在线测试");
        ui->textOfQuiz->setText("<html><head/><body><p>提供编译原理相关的在线测试题目，包括选择题、填空题、编程题等，帮助用户检验学习成果</p></body></html>");
    }
}

void mainpage::initalMainPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/home");

    connect(ui->course,&clicklabel::clicked,this,&mainpage::selectLabel);
    connect(ui->program,&clicklabel::clicked,this,&mainpage::selectLabel);
    connect(ui->quiz,&clicklabel::clicked,this,&mainpage::selectLabel);

    connect(ui->btnOfCourse,&QPushButton::clicked,this,[=](){ selectLabel(ui->course); });
    connect(ui->btnOfProgram,&QPushButton::clicked,this,[=](){ selectLabel(ui->program); });
    connect(ui->btnOfQuiz,&QPushButton::clicked,this,[=](){ selectLabel(ui->quiz); });
}
